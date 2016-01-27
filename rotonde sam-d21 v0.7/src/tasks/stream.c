/*
* stream.c
*
* Created: 20/01/2016 16:29:35
*  Author: GroundLayer
*/

#include "tasks/stream.h"
#include "tasks/modules.h"

enum feature_code {
	STOP = 0x00,
	START = 0x01,
	SEND_DEF = 0xFF
};

typedef enum Rontonde_receiver_states {
	STREAM_PARSING = 0,
	JSMN_PARSING,
	TYPE_PARSING,
	PAYLOAD_FIELD,
	IDENTIFIER_PARSING,
	DATA_FIELD,
	DATA_PARSING
} receiver_states;

static struct {
	receiver_states state;
	binary_parser bin_parser;
	jsmn_parser jsmn_Parser;
	jsmntok_t jsmn_Tokens[JSMN_MAX_TOKENS]; /* We expect no more than JSON_MAX_TOKENS tokens */
	int jsmn_num_tokens;
	instance inst;
} stream_parser;

// Private functions

void enqueueJSON(char *HID_buffer, uint16_t length);
receiver_states parser(void);

//Private variables

static bool rotonde_started = false;

// Tasks

void receiver_stream(void *pvParameters){
	
	portBASE_TYPE xStatus = pdFAIL;
	uint8_t USB_report[UDI_HID_REPORT_OUT_SIZE];
	
	binary_parser_init(&stream_parser.bin_parser);
	
	for (;;) {
		
		xStatus = xQueueReceive( stream_out, USB_report, 1000 / portTICK_RATE_MS );
		if( xStatus == pdPASS ){
			for(int i = 0 ; i < UDI_HID_REPORT_OUT_SIZE ; ++i){
				if(binary_parser_process_byte(&stream_parser.bin_parser, USB_report[i]) == PARSER_STATE_COMPLETE) {
					stream_parser.state = JSMN_PARSING;
					while(parser() != STREAM_PARSING);
					break;
				}
			}
		}
	}
}

char HID_buffer[HID_PACKET_HEADER_AND_TAIL_LENGTH + HID_PACKET_MAX_DATA_SIZE];
char Set_feature[UDI_HID_REPORT_FEATURE_SIZE];

void sender_stream(void *pvParameters){
	
	portBASE_TYPE xStatus_report;
	portBASE_TYPE xStatus_feature;
	instance inst;

	for (;;) {
		
		if(HAL_usb_is_enabled()) {
			
			// Got JSON
			if(rotonde_started){
				xStatus_report = xQueueReceive( module_out, &inst, 1000 / portTICK_RATE_MS );
				if( xStatus_report == pdPASS){
					
					uint16_t length_data =  JSON_rotonde_encoder(HID_buffer + HID_PACKET_HEADER_LENGTH, inst.def->type, inst.def->identifier, inst.def->data_names, inst.def->data_types, inst.values, inst.def->n_data);
					enqueueJSON(HID_buffer, length_data);
					release_instance(inst);
				}
			}

			// Got feature
			xStatus_feature = xQueueReceive(set_feature_queue, &Set_feature, 0);
			if(xStatus_feature == pdPASS) {
				
				xQueueReset(module_out);
				xQueueReset(module_in);
				
				switch(Set_feature[0]){
					case STOP :
					rotonde_started = false;
					break;
					
					case START :
					rotonde_started = true;
					break;
					
					case SEND_DEF :
					{
					definition* def;
					int index_registrar = 0;
					while((def = get_definition_by_index(index_registrar)) != NULL){
						uint16_t length_data = JSON_def_encoder(HID_buffer + HID_PACKET_HEADER_LENGTH , def->type , def->identifier , def->data_names , def->data_types , def->n_data);
						enqueueJSON(HID_buffer, length_data);
						index_registrar++;
					}
					}
					rotonde_started = true;
					break;
					
					default :
					break;
				}
			}
			} else {
			rotonde_started = false;
			vTaskDelay(10);
		}
	}
}

receiver_states parser(void){
	
	switch(stream_parser.state) {
		case JSMN_PARSING :
		jsmn_init(&stream_parser.jsmn_Parser);
		stream_parser.jsmn_num_tokens = 0;
		
		stream_parser.jsmn_num_tokens = jsmn_parse(&stream_parser.jsmn_Parser, stream_parser.bin_parser.buffer , stream_parser.bin_parser.PacketSize, stream_parser.jsmn_Tokens, sizeof(stream_parser.jsmn_Tokens)/sizeof(stream_parser.jsmn_Tokens[0]));
		if( stream_parser.jsmn_num_tokens < 0){
			//Failed to decode json
			stream_parser.state = STREAM_PARSING;
			break;
		}
		stream_parser.state = TYPE_PARSING;
		break;
		
		case TYPE_PARSING :
		if(!JSON_check_field(stream_parser.jsmn_Tokens , stream_parser.bin_parser.buffer , TYPE_POSITION , TYPE)) {
			stream_parser.state = STREAM_PARSING;
			break;
		}
		if(!JSON_check_field(stream_parser.jsmn_Tokens , stream_parser.bin_parser.buffer , TYPE_POSITION +1 , ACTION)){
			stream_parser.state = STREAM_PARSING;
			break;
		}
		stream_parser.state = PAYLOAD_FIELD;
		break;
		
		case PAYLOAD_FIELD :
		if(!JSON_check_field(stream_parser.jsmn_Tokens , stream_parser.bin_parser.buffer , PAYLOAD_POSITION , PAYLOAD)){
			stream_parser.state = STREAM_PARSING;
			break;
		}
		stream_parser.state = IDENTIFIER_PARSING;
		break;
		
		case IDENTIFIER_PARSING :
		if(!JSON_check_field(stream_parser.jsmn_Tokens , stream_parser.bin_parser.buffer , IDENTIFIER_POSITION , IDENTIFIER)){
			stream_parser.state = STREAM_PARSING;
			break;
		}
		int id_start = stream_parser.jsmn_Tokens[IDENTIFIER_POSITION +1].start;
		int id_end = stream_parser.jsmn_Tokens[IDENTIFIER_POSITION +1].end;
		definition *def = get_definition_by_name_n(stream_parser.bin_parser.buffer + id_start , id_end - id_start);
		if(def == NULL){
			stream_parser.state = STREAM_PARSING;
			break;
		}
		//Check that the correct number of tokens is present
		if(def->n_data*2 != stream_parser.jsmn_num_tokens - START_DATA_POSITION){
			stream_parser.state = STREAM_PARSING;
			break;
		}
		
		stream_parser.inst = create_instance_from_definition(def);
		
		stream_parser.state = DATA_FIELD;
		break;
		
		case DATA_FIELD :
		if(!JSON_check_field(stream_parser.jsmn_Tokens , stream_parser.bin_parser.buffer , DATA_POSITION , DATA)){
			stream_parser.state = STREAM_PARSING;
			break;
		}
		stream_parser.state = DATA_PARSING;
		break;
		
		case DATA_PARSING :
		{
		bool data_parsing_result = false;
		
		for(int i = 0 ; i < stream_parser.inst.def->n_data ; i++) {
			for(int j = 0 ; j < stream_parser.inst.def->n_data ; j++) {
				if(JSON_check_field(stream_parser.jsmn_Tokens , stream_parser.bin_parser.buffer , START_DATA_POSITION + 2*i , stream_parser.inst.def->data_names[j])){
					int data_start = stream_parser.jsmn_Tokens[START_DATA_POSITION + 2*i +1].start;
					int data_end = stream_parser.jsmn_Tokens[START_DATA_POSITION + 2*i + 1].end;
					if(JSON_recover_data(stream_parser.inst.def->data_types[j] , stream_parser.inst.values[j] , stream_parser.bin_parser.buffer + data_start ,  data_end - data_start )) {
						data_parsing_result = true;
					}
				}
			}
			if(data_parsing_result == false) {
				//Error somewhere in parsing
				stream_parser.state = STREAM_PARSING;
				release_instance(stream_parser.inst);
				break;
			}
		}
		}
		
		//The end
		// TODO : get this part in the task
		if (xQueueSendToBack(module_in , &stream_parser.inst , 10 / portTICK_RATE_MS ) != pdPASS) {
			release_instance(stream_parser.inst);
		}
		stream_parser.state = STREAM_PARSING;
		break;
		
		case STREAM_PARSING:
		break;
	}
	return stream_parser.state;
}

// Utils

void enqueueJSON(char *buffer, uint16_t length) {
	if(length > 0 && length < HID_PACKET_MAX_DATA_SIZE) {
		length = binary_parser_encode(buffer , length);

		for(int i = 0 ; i < length ; i+=UDI_HID_REPORT_IN_SIZE){
			char USB_report[UDI_HID_REPORT_IN_SIZE];
			memset(USB_report , 0x00 , UDI_HID_REPORT_IN_SIZE);
			memcpy(USB_report , buffer +i , min(UDI_HID_REPORT_IN_SIZE , length - i));
			if( xQueueSendToBack( stream_in, USB_report, 10 / portTICK_RATE_MS ) != pdPASS ){
				//Can hook here for debug
			}
		}
	}
}
