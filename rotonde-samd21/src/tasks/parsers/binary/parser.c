/**
******************************************************************************
* @file       parser.c
* @author     GroundLayer http://groundlayer.org Copyright (C) 2016.
* @author     Hackerloop http://hackerloop.com/
* @brief      Include file of the HID Parser
* @brief      Process Byte by Byte the stream comming from a stream
*
*****************************************************************************/

#include "parser.h"

void binary_parser_init(binary_parser* Parser) {
	Parser->state = PARSER_STATE_SYNC;
}

Parser_state binary_parser_process_byte(binary_parser* parser , uint8_t rxbyte){
	
	//Update packet byte count, reset to error if length is too long (something wrong has happened)
	if(parser->rxPacketLength < HID_PACKET_MAX_DATA_SIZE + HID_PACKET_HEADER_AND_TAIL_LENGTH) {
		parser->rxPacketLength++;
		} else {
		parser->state = PARSER_STATE_ERROR;
		parser->rxPacketLength = 0;
	}
	
	//Check state of the Parser and reset it if complete or error
	if( parser->state == PARSER_STATE_ERROR || parser->state == PARSER_STATE_COMPLETE) {
		parser->state = PARSER_STATE_SYNC;
		parser->rxPacketLength = 0;
	}
	
	//Parser state machine
	switch (parser->state){
		
		//Search for sync character
		case PARSER_STATE_SYNC:
		
		if(rxbyte != HID_SYNC_VAL) {
			break;
		}
		//Update state and packet length once SYNC_VAL received
		parser->rxPacketLength = 1;
		parser->rxcount = 0;
		// Initialize and update the CRC
		parser->checksum = CRC_updateByte(0, rxbyte);
		parser->state = PARSER_STATE_TYPE;
		break;
		
		//Check type of data sent
		case PARSER_STATE_TYPE:
		
		// update the CRC
		parser->checksum = CRC_updateByte(parser->checksum, rxbyte);
		
		//Check Revision of the data to Parse
		if(((rxbyte & HID_VERSION_MASK ) >> 6) != HID_ROTONDE_VERSION) {
			parser->state = PARSER_STATE_ERROR;
			break;
		}
		
		parser->type = rxbyte & 0x3F;
		parser->rxcount = 0;
		parser->state = PARSER_STATE_SIZE;
		break;
		
		//Get the size of the data
		case PARSER_STATE_SIZE:
		
		// update the CRC
		parser->checksum = CRC_updateByte(parser->checksum, rxbyte);
		
		if(parser->rxcount == 0) {
			parser->PacketSize = rxbyte;
			parser->rxcount++;
			break;
			} else {
			parser->PacketSize = (rxbyte << 8) + parser->PacketSize;
		}
		
		if(parser->PacketSize < HID_PACKET_HEADER_AND_TAIL_LENGTH || parser->PacketSize > HID_PACKET_HEADER_AND_TAIL_LENGTH + HID_PACKET_MAX_DATA_SIZE) {
			//Error in size
			parser->state = PARSER_STATE_ERROR;
			break;
		}
		
		parser->rxcount = 0;
		//Reset data buffer to blank
		memset(parser->buffer , ' ' , HID_PACKET_MAX_DATA_SIZE);
		parser->state = PARSER_STATE_DATA;
		
		break;
		
		//Recover the data
		case PARSER_STATE_DATA:
		
		// update the CRC
		parser->checksum = CRC_updateByte(parser->checksum, rxbyte);
		
		parser->buffer[parser->rxcount++] = rxbyte;
		if(parser->rxcount < parser->PacketSize) {
			break;
		}
		
		parser->rxcount = 0;
		parser->state = PARSER_STATE_CS;
		break;
		
		case  PARSER_STATE_CS:
		//Check the CRC
		if(rxbyte != parser->checksum){
			//Parser_proc->state = PARSER_STATE_ERROR;
			//break;
		}
		
		//Check the length of the packet
		if(parser->rxPacketLength != parser->PacketSize + HID_PACKET_HEADER_AND_TAIL_LENGTH){
			parser->state = PARSER_STATE_ERROR;
			break;
		}
		
		parser->rxcount = 0;
		parser->state = PARSER_STATE_COMPLETE;
		break;
		
		//Should not get here, so error
		default:
		parser->state = PARSER_STATE_ERROR;
	}
	return  parser->state;
}

uint16_t binary_parser_encode(char* buffer , uint16_t data_length){
	buffer[0] = HID_SYNC_VAL;
	buffer[1] = HID_ROTONDE_VERSION << HID_VERSION_OFFSET;
	*((uint16_t *)&buffer[2]) = data_length;
	
	uint8_t crc = CRC_updateByte(0, buffer[0]);
	for(int i = 1 ; i < data_length + HID_PACKET_HEADER_LENGTH + data_length ;++i){
		CRC_updateByte(crc, buffer[i]);
	}
	buffer[data_length + HID_PACKET_HEADER_AND_TAIL_LENGTH - 1] = crc;
	
	return data_length + HID_PACKET_HEADER_AND_TAIL_LENGTH;
}