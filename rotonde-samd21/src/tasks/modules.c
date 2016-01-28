/*
 * modules.c
 *
 * Created: 20/01/2016 18:07:02
 *  Author: GroundLayer
 */ 

#include "tasks/modules.h"
#include "modules/modules_init.h"

#include "registrar/registrar.h"

QueueHandle_t module_in;
QueueHandle_t module_out;

void modules(void *pvParameters){
	
	instance inst;
	portBASE_TYPE xStatus;
	
	init_modules();
	
	for (;;) {
		xStatus = xQueueReceive( module_in , &inst , 0 );
		if( xStatus == pdPASS ) {
			inst.def->action(inst);
			release_instance(inst);
		}
		tick_modules();
	}
}