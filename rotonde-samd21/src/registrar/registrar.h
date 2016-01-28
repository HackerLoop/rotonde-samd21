/*
 * registrar.h
 *
 * Created: 20/01/2016 17:06:32
 *  Author: GroundLayer
 */ 


#ifndef REGISTRAR_H_
#define REGISTRAR_H_

#include "asf.h"
#include "config/conf_rotonde.h"

#define MAX_NUMBER_OF_DEFINITIONS 6

typedef const char* data_name;
typedef void* data_value;

typedef enum {
	STRING,
	INT,
	INT_ARRAY,
	LONG,
	DOUBLE,
	FLOAT
} data_type;

// Object "instances"
struct s_instance;
typedef struct {
	const char* type;
	const char* identifier;
	data_name* data_names;
	data_type* data_types;
	int n_data;
	void (*action)(struct s_instance);
	void (*tick)(void);
	uint tickdelay;
	TickType_t last_tick; 
} definition;

// TODO refactor to minimize malloc by using internal data buffer, values pointers would point to it.
typedef struct s_instance {
	definition* def;
	data_value* values;
} instance;

definition* register_definition(const char* type , const char* identifier , data_name* data_names , data_type* data_types , int n_data , void (*action)(instance), void (*tick)(void), uint tickdelay_ms);
definition* get_definition_by_name(const char* name);
definition* get_definition_by_name_n(const char* name, uint length);
definition* get_definition_by_index(int registrar_index);

instance create_instance(const char *name);
instance create_instance_from_definition(definition *def);
void release_instance(instance inst);

void tick_modules(void);

#endif /* REGISTRAR_H_ */