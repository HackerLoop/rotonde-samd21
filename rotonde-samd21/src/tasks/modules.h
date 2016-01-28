/*
 * modules.h
 *
 * Created: 20/01/2016 18:07:09
 *  Author: GroundLayer
 */ 


#ifndef MODULES_H_
#define MODULES_H_

#include "asf.h"
#include "config/conf_rotonde.h"
#include "registrar/registrar.h"

#define MODULES_STACK_SIZE     800
#define MODULES_PRIORITY		(2)

#define MODULE_QUEUE_LENGTH 10
#define MODULE_QUEUE_ITEM_SIZE sizeof(instance)
extern QueueHandle_t module_in;
extern QueueHandle_t module_out;

void modules(void *pvParameters);

#endif /* MODULES_H_ */