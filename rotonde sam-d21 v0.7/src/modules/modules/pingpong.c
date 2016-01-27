/*
* pingpong.c
*
* Created: 20/01/2016 18:40:42
*  Author: GroundLayer
*/

#include "modules/modules/pingpong.h"

#include "tasks/modules.h"
#include "registrar/registrar.h"

#define MODULE_NAME_PING "PING"
#define MODULE_NAME_PONG "PONG"
#define MODULE_DATA_SIZE 1

#define MODULE_N "n"

data_name names[MODULE_DATA_SIZE] = { MODULE_N };
data_type types[] = { INT };

void Ping_action(instance inst);
void Pong_tick(void);
instance Pong_create_instance(int n);

definition *pingDef;
definition *pongDef;

void Pinpong_module_init(void) {
	pingDef = register_definition(ACTION , MODULE_NAME_PING , names, types, MODULE_DATA_SIZE, Ping_action, NULL , 0);
	pongDef = register_definition(EVENT , MODULE_NAME_PONG , names, types, MODULE_DATA_SIZE, NULL, Pong_tick , 1000);
}

void Ping_action(instance inst) {
	int n = *(int *)inst.values[0];
	instance resp = Pong_create_instance(n + 1);
	if(resp.def != NULL && resp.values != NULL){
		portBASE_TYPE xStatus = xQueueSendToBack(module_out, &resp , 10 / portTICK_RATE_MS );
		if(xStatus != pdPASS){
			release_instance(resp);
		}
	}
}

void Pong_tick(void) {
	instance inst = create_instance(MODULE_NAME_PONG);
	if(inst.def != NULL && inst.values != NULL){
		(*(int*) inst.values[0]) = 42;
		portBASE_TYPE xStatus = xQueueSendToBack(module_out, &inst , 10 / portTICK_RATE_MS );
		if(xStatus != pdPASS){
			release_instance(inst);
		}
	}
}

instance Pong_create_instance(int n) {
	instance inst = create_instance(MODULE_NAME_PONG);
	if(inst.def == NULL) {
		return inst;
	}
	(*(int*) inst.values[0]) = n;
	return inst;
}