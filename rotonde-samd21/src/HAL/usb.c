/*
 * usb.c
 *
 * Created: 20/01/2016 16:11:33
 *  Author: GroundLayer
 */ 

#include "HAL/usb.h"
#include "string.h"

static volatile bool usb_enabled = false;

//Local copy of data going in and out of the MCU
static uint8_t USB_report_out[UDI_HID_REPORT_OUT_SIZE];
static uint8_t USB_report_in[UDI_HID_REPORT_IN_SIZE];
static uint8_t USB_set_feature[UDI_HID_REPORT_FEATURE_SIZE];

#define STREAM_OUT_QUEUE_LENGTH 10
#define STREAM_OUT_QUEUE_ITEM_SIZE sizeof(USB_report_out)
#define STREAM_IN_QUEUE_LENGTH 10
#define STREAM_IN_QUEUE_ITEM_SIZE sizeof(USB_report_in)
#define SET_FEATURE_QUEUE_LENGTH 2
#define SET_FEATURE_QUEUE_ITEM_SIZE sizeof(USB_set_feature)

QueueHandle_t stream_out;
QueueHandle_t stream_in;
QueueHandle_t set_feature_queue;

bool HAL_usb_init(void){
	stream_out = xQueueCreate( STREAM_OUT_QUEUE_LENGTH, STREAM_OUT_QUEUE_ITEM_SIZE );
	stream_in = xQueueCreate( STREAM_IN_QUEUE_LENGTH, STREAM_IN_QUEUE_ITEM_SIZE );
	set_feature_queue = xQueueCreate( SET_FEATURE_QUEUE_LENGTH, SET_FEATURE_QUEUE_ITEM_SIZE );
	if( stream_out == NULL || stream_in == NULL )
	{
		return false;
	}
	// Start USB stack to authorize VBus monitoring
	udc_start();
	return true;
}

bool HAL_usb_enable(void){
	usb_enabled = true;
	//reset queues to remove old data
	xQueueReset(stream_out);
	xQueueReset(stream_in);
	xQueueReset(set_feature_queue);
	return true;
}

bool HAL_usb_is_enabled(void){
	return usb_enabled;
}

void HAL_usb_disable(void){
	usb_enabled = false;
	xQueueReset(stream_out);
	xQueueReset(stream_in);
	xQueueReset(set_feature_queue);
}

/**
* Interrupt filling the USB report out queue from data arriving from USB
*/
void HAL_usb_receive_report_out(uint8_t *report){
	if(usb_enabled){
		portBASE_TYPE xHigherpriotaskwoken = pdFALSE;
		for(uint8_t i = 0 ; i < UDI_HID_REPORT_OUT_SIZE ; ++i){
			USB_report_out[i] = (char) report[i];
		}
		xQueueSendToBackFromISR( stream_out, USB_report_out , &xHigherpriotaskwoken );
	}
}

/**
* Start of frame interrupt
*/
void HAL_usb_sof_action(void){
	if(usb_enabled){
		HAL_usb_send_report_in();
	}
}

bool HAL_usb_send_report_in(void){
	portBASE_TYPE xStatus;
	uint8_t USB_report[UDI_HID_REPORT_OUT_SIZE];
	
	//Check if data if present in the queue
	xStatus = xQueuePeekFromISR( stream_in, USB_report);
	if ( xStatus == pdPASS ) {
		//try to send report
		if(udi_hid_generic_send_report_in(USB_report)){
			portBASE_TYPE xHigherpriotaskwoken = pdFALSE;
			//remove data from queue only if report has been sent
			xQueueReceiveFromISR( stream_in, USB_report, &xHigherpriotaskwoken );
		} else return false;
	}
	return true;
}


void HAL_usb_set_feature(uint8_t *report_feature){
	if(usb_enabled){
		portBASE_TYPE xHigherpriotaskwoken = pdFALSE;
		for(uint8_t i = 0 ; i < UDI_HID_REPORT_FEATURE_SIZE ; ++i){
			USB_set_feature[i] = (char) report_feature[i];
		}
		xQueueSendToBackFromISR( set_feature_queue, USB_set_feature , &xHigherpriotaskwoken );
	}
}