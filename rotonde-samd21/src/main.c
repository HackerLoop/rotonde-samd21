#include <asf.h>

#include "HAL/usb.h"

#include "tasks/modules.h"
#include "tasks/stream.h"

void intar(void){
	pin_toggle(PIN_PA21);
}

int main (void)
{
	irq_initialize_vectors();
	cpu_irq_enable();
	
	system_init();
	sleepmgr_init();
	
	HAL_usb_init();
	
	module_in = xQueueCreate(MODULE_QUEUE_LENGTH , MODULE_QUEUE_ITEM_SIZE);
	module_out = xQueueCreate(MODULE_QUEUE_LENGTH , MODULE_QUEUE_ITEM_SIZE);
	
	if( module_in == NULL || module_out == NULL){
		/* The queue could not be created. */
		while(1);
	}
	
	// Create tasks
	xTaskCreate(&receiver_stream, (const char *)"receiver_stream", RECEIVER_STREAM_STACK_SIZE, NULL , RECEIVER_STREAM_PRIORITY , NULL);
	xTaskCreate(&sender_stream, (const char *)"sender_stream", SENDER_STREAM_STACK_SIZE, NULL , SENDER_STREAM_PRIORITY , NULL);
	xTaskCreate(&modules, (const char *)"modules", MODULES_STACK_SIZE, NULL , MODULES_PRIORITY , NULL);

	//Start FreeRTOS scheduler
	vTaskStartScheduler();

	/* Code should never get here */
	while (1) {
	}
}
