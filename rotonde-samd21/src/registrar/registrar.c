/*
 * registrar.c
 *
 * Created: 20/01/2016 17:06:25
 *  Author: GroundLayer
 */ 

#include "tasks/modules.h"
#include "registrar/registrar.h"
#include "string.h"

static uint8_t n_definitions = 0;
definition registry[MAX_NUMBER_OF_DEFINITIONS];

definition* register_definition(const char* type , const char* identifier , data_name* data_names , data_type* data_types , int n_data , void (*action)(instance), void (*tick)(void), uint tickdelay_ms){
	
	//Check if max number of modules is already reached
	if(n_definitions >= MAX_NUMBER_OF_DEFINITIONS) {
		return NULL;
	}
	registry[n_definitions].type = type;
	registry[n_definitions].identifier = identifier;
	registry[n_definitions].data_names = data_names;
	registry[n_definitions].data_types = data_types;
	registry[n_definitions].n_data = n_data;
	registry[n_definitions].action = action;
	registry[n_definitions].tick = tick;
	registry[n_definitions].tickdelay = tickdelay_ms / portTICK_RATE_MS;
	registry[n_definitions].last_tick = xTaskGetTickCount();
	n_definitions++;
	return &registry[n_definitions];
}

definition* get_definition_by_name(const char* name) {
	for(int i = 0 ; i < n_definitions;  ++i){
		if(strcmp(name , registry[i].identifier) == 0){
			return &registry[i];
		}
	}
	return NULL;
}

definition* get_definition_by_name_n(const char* name, uint length) {
	for(int i = 0 ; i < n_definitions;  ++i){
		if(strncmp(name , registry[i].identifier, length) == 0){
			return &registry[i];
		}
	}
	return NULL;
}

instance create_instance(const char *name) {
	definition* def = get_definition_by_name(name);
	return create_instance_from_definition(def);
}

instance create_instance_from_definition(definition *def) {
	
	instance inst = {.def = def};

	if(def == NULL) {
		return inst; // def is NULL on failure :/
	}
	
	inst.values = malloc(def->n_data * sizeof(void*));
	
	for(int i = 0 ; i < def->n_data ; i++){
		
		void* data;
		switch(def->data_types[i]){
			case STRING :
			break;
			
			case INT :
			data = malloc(sizeof(int));
			inst.values[i] = data;
			break;
			
			case INT_ARRAY :
			
			break;
			
			case LONG :
			
			break;
			
			case DOUBLE :
			data = malloc(sizeof(double));
			inst.values[i] = data;
			break;
			
			case FLOAT :
			data = malloc(sizeof(float));
			inst.values[i] = data;
			break;
		}
	}
	return inst;
}

void release_instance(instance inst){
	for(int i = 0 ; i < inst.def->n_data ; i++){
		free(inst.values[i]);
	}
	free(inst.values);
}

definition* get_definition_by_index(int index){
	if(index < n_definitions){
		return &registry[index];
	}
	return NULL;
}

void tick_modules(void) {
	TickType_t current_tick = xTaskGetTickCount();
	for(int i = 0 ; i < n_definitions;  ++i){
		if (registry[i].tick != NULL && current_tick - registry[i].last_tick >= registry[i].tickdelay) {
			registry[i].tick();
			registry[i].last_tick = current_tick;
		}
	}
}