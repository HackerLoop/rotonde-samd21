/*
 * conf_rotonde.h
 *
 * Created: 20/01/2016 16:30:17
 *  Author: GroundLayer
 */ 


#ifndef CONF_ROTONDE_H_
#define CONF_ROTONDE_H_

#define JSON_DEF_HEADER "{\"type\":\"def\",\"payload\":{\"type\":\"%s\",\"identifier\":\"%s\",\"fields\":["
#define JSON_DEF_DATA	"{\"name\":\"%s\",\"type\":\"%s\"},"
#define JSON_DEF_TAIL	"]}}"

#define JSON_ROTONDE_HEADER "{\"type\":\"%s\",\"payload\":{\"identifier\":\"%s\",\"data\":{"
#define JSON_ROTONDE_TAIL "}}}"

#define TYPE		"type"
#define ACTION		"action"
#define EVENT		"event"
#define PAYLOAD		"payload"
#define IDENTIFIER	"identifier"
#define DATA		"data"

#define TYPE_STRING		"string"
#define TYPE_INT		"int"
#define TYPE_INT_ARRAY	"int[]"
#define TYPE_LONG		"long"
#define TYPE_DOUBLE		"double"
#define TYPE_FLOAT		"float"

#define TYPE_POSITION			1
#define PAYLOAD_POSITION		3
#define IDENTIFIER_POSITION		5
#define DATA_POSITION			7
#define START_DATA_POSITION		9
#define JSMN_MAX_TOKENS			30

#define HID_PACKET_HEADER_LENGTH	4
#define HID_PACKET_TAIL_LENGTH		1
#define HID_PACKET_HEADER_AND_TAIL_LENGTH	HID_PACKET_HEADER_LENGTH + HID_PACKET_TAIL_LENGTH
#define HID_PACKET_MAX_DATA_SIZE	256
#define HID_SYNC_VAL				0x3C
#define HID_VERSION_MASK			0xC0
#define HID_VERSION_OFFSET			6
#define HID_ROTONDE_VERSION			1

#endif /* CONF_ROTONDE_H_ */