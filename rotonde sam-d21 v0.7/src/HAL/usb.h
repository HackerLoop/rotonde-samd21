/*
 * usb.h
 *
 * Created: 20/01/2016 16:11:44
 *  Author: GroundLayer
 */ 


#ifndef USB_H_
#define USB_H_

#include "asf.h"

//Define USB clock generator
#  define UDD_CLOCK_GEN      GCLK_GENERATOR_3

//USB queues used to transmit data to the tasks
extern QueueHandle_t stream_out;
extern QueueHandle_t stream_in;
extern QueueHandle_t set_feature_queue;

bool HAL_usb_init(void);
bool HAL_usb_enable(void);
bool HAL_usb_is_enabled(void);
void HAL_usb_disable(void);
void HAL_usb_receive_report_out(uint8_t *report);
void HAL_usb_sof_action(void);
bool HAL_usb_send_report_in(void);
void HAL_usb_set_feature(uint8_t *report_feature);

#endif /* USB_H_ */