/*
 * stream.h
 *
 * Created: 20/01/2016 16:29:42
 *  Author: GroundLayer
 */ 


#ifndef STREAM_H_
#define STREAM_H_

#include "asf.h"

#include "config/conf_rotonde.h"

#include "HAL/usb.h"

#include "tasks/parsers/binary/parser.h"
#include "tasks/parsers/json/parser.h"
#include "tasks/parsers/json/jsmn.h"

#include "registrar/registrar.h"

#define RECEIVER_STREAM_STACK_SIZE 400
#define RECEIVER_STREAM_PRIORITY   (2)
#define SENDER_STREAM_STACK_SIZE 400
#define SENDER_STREAM_PRIORITY   (2)

void receiver_stream(void *pvParameters);
void sender_stream(void *pvParameters);

#endif /* STREAM_H_ */