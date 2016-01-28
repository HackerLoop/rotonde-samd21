/**
******************************************************************************
* @file       parser.h
* @author     GroundLayer http://groundlayer.org Copyright (C) 2016.
* @author     Hackerloop http://hackerloop.com/
* @brief      Include file of the HID Parser
* @brief      Process Byte by Byte the stream comming from a stream
*
*****************************************************************************/


#ifndef BINARY_PARSER_H_
#define BINARY_PARSER_H_

#include "asf.h"
#include "string.h"
#include "crc.h"

#include "config/conf_rotonde.h"

typedef enum {
	PARSER_STATE_ERROR=0,
	PARSER_STATE_SYNC,
	PARSER_STATE_TYPE,
	PARSER_STATE_SIZE,
	PARSER_STATE_DATA,
	PARSER_STATE_CS,
	PARSER_STATE_COMPLETE
} Parser_state;

typedef struct _binary_parser {
	Parser_state state;
	uint16_t rxPacketLength;
	uint8_t type;
	uint8_t objectID;
	uint16_t PacketSize;
	uint8_t rxcount;
	uint8_t checksum;
	char buffer[HID_PACKET_MAX_DATA_SIZE];
} binary_parser;

void binary_parser_init(binary_parser* parser);
Parser_state binary_parser_process_byte(binary_parser* parser , uint8_t rxbyte);
uint16_t binary_parser_encode(char* buffer , uint16_t data_length);

#endif /* BINARY_PARSER_H_ */