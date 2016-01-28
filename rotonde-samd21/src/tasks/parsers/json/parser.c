/*
 * parser.c
 *
 * Created: 20/01/2016 17:03:24
 *  Author: GroundLayer
 */ 

#include "tasks/parsers/json/parser.h"

#include "string.h"

bool JSON_check_field(jsmntok_t* jsmn_Tokens , char* data , int token_position , const char* field_template) {
	
	unsigned int field_start_index = jsmn_Tokens[token_position].start;
	unsigned int field_length = jsmn_Tokens[token_position].end - jsmn_Tokens[token_position].start;
	
	if(field_length == strlen(field_template) && strncmp(data + field_start_index , field_template , field_length) == 0){
		return true;
	} else return false;
}

bool JSON_recover_data(data_type type, void* data_values,  char* JSON_data , int JSON_data_length ){
	
	//Make a local copy of the string data
	char data[JSON_data_length];
	strncpy(data , JSON_data , JSON_data_length);
	
	switch (type){
		
		case STRING :
		strncpy((char*) data_values , data , JSON_data_length);
		break;
		
		case INT :
		*((int*) data_values) = atoi(data);
		break;
		
		case LONG :
		*((long*) data_values) = atol(data);
		break;
		
		case DOUBLE :
		*((double*) data_values) = atof(data);
		break;
		
		case FLOAT :
		*((float*) data_values) = atof(data);
		break;
		
		
		//Undefined data type
		default:
		return false;
	}
	return true;
}

uint16_t JSON_def_encoder(char* buffer ,const char* type , const char* identifier , data_name* data_names , data_type* data_types , int data_length){
	uint16_t index_json = 0;
	index_json += sprintf(buffer + index_json , JSON_DEF_HEADER , type , identifier);
	
	for(int i = 0 ; i < data_length ; ++i){
		
		switch(data_types[i]) {
			case STRING :
			index_json += sprintf(buffer + index_json , JSON_DEF_DATA , data_names[i] , TYPE_STRING);
			break;
			
			case INT :
			index_json += sprintf(buffer + index_json , JSON_DEF_DATA , data_names[i] , TYPE_INT);
			break;
			
			case INT_ARRAY :
			index_json += sprintf(buffer + index_json , JSON_DEF_DATA , data_names[i] , TYPE_INT_ARRAY);
			break;
			
			case LONG :
			index_json += sprintf(buffer + index_json , JSON_DEF_DATA , data_names[i] , TYPE_LONG);
			break;
			
			case DOUBLE :
			index_json += sprintf(buffer + index_json , JSON_DEF_DATA , data_names[i] , TYPE_DOUBLE);
			break;
			
			case FLOAT :
			index_json += sprintf(buffer + index_json , JSON_DEF_DATA , data_names[i] , TYPE_FLOAT);
			break;
			
			//Should not get here
			default:
			return -1;
			
		}
	}
	//Remove the last "," if data is present (JSON formating)
	if(data_length > 0) {
		index_json--;
	}
	index_json += sprintf(buffer + index_json , JSON_DEF_TAIL);
	return index_json;
	
}

uint16_t JSON_rotonde_encoder(char* buffer ,const char* type , const char* identifier , data_name* data_names , data_type* data_types , data_value* data_values , int data_length){
	uint16_t index_json = 0;
	index_json += sprintf(buffer + index_json , JSON_ROTONDE_HEADER , type , identifier);
	for(int i = 0 ; i < data_length ; ++i){
		
		switch(data_types[i]) {
			
			case STRING :
			//index_json += sprintf(buffer + index_json , "\"%s\":\"%s\"," , data_names[i] , (*(char*) data_values[i]));
			break;
			
			case INT :
			index_json += sprintf(buffer + index_json , "\"%s\":%d," , data_names[i] , (*(int*) data_values[i]) );
			break;
			
			case INT_ARRAY :
			//index_json += sprintf(buffer + index_json , JSON_DEF_DATA , data_names[i] , );
			break;
			
			case LONG :
			//index_json += sprintf(buffer + index_json , "\"%s\":%f," , data_names[i] , (*(long*) data_values[i]) );
			break;
			
			case DOUBLE :
			index_json += sprintf(buffer + index_json , "\"%s\":%f,", data_names[i] , (*(double*) data_values[i]));
			break;
			
			case FLOAT :
			index_json += sprintf(buffer + index_json , "\"%s\":%f," , data_names[i] , (*(float*) data_values[i]));
			break;
			
			//Should not get here
			default:
			return -1;
			
		}
	}
	if(data_length > 0) {
		index_json--;
	}
	index_json += sprintf(buffer + index_json , JSON_ROTONDE_TAIL);
	return index_json;
}