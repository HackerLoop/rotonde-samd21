/*
 * parser.h
 *
 * Created: 20/01/2016 17:03:32
 *  Author: GroundLayer
 */ 


#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include "asf.h"
#include "jsmn.h"

#include "config/conf_rotonde.h"

#include "registrar/registrar.h"

bool JSON_check_field(jsmntok_t* jsmn_Tokens , char* data , int token_position , const char* field_template);
bool JSON_recover_data(data_type type, void* data_value,  char* JSON_data , int JSON_data_length);
uint16_t JSON_def_encoder(char* buffer ,const char* type , const char* identifier , data_name* data_names , data_type* data_types , int data_length);
uint16_t JSON_rotonde_encoder(char* buffer ,const char* type , const char* identifier , data_name* data_names , data_type* data_types , data_value* data_values , int data_length);

#endif /* JSON_PARSER_H_ */