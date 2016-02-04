/*
 * interrupts.c
 *
 * Created: 04/02/2016 16:04:58
 *  Author: GroundLayer
 */ 

#include "digital_pins.h"

void pinmode_in(uint32_t pin, enum port_pin_pull input_pull){
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	/* Set buttons direction */
	pin_conf.direction  = PORT_PIN_DIR_INPUT;
	pin_conf.input_pull = input_pull;
	port_pin_set_config(pin, &pin_conf);
}

void attach_interrupt(uint32_t pin, enum port_pin_pull input_pull,enum extint_detect detection_criteria,bool filter_input_signal, uint8_t eic, void (*callback)(void)){
	pinmode_in(pin, input_pull);
	struct extint_chan_conf eint_chan_conf;
	extint_chan_get_config_defaults(&eint_chan_conf);
	
	eint_chan_conf.gpio_pin           = pin;
	eint_chan_conf.gpio_pin_mux       = 0;
	eint_chan_conf.detection_criteria = detection_criteria;
	eint_chan_conf.filter_input_signal = filter_input_signal;
	extint_chan_set_config(eic, &eint_chan_conf);
		
	extint_register_callback(callback, eic ,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(eic, EXTINT_CALLBACK_TYPE_DETECT);
}

void pinmode_out(uint32_t pin){
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	/* Set buttons direction */
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(pin, &pin_conf);
}



void pin_set(uint32_t pin, bool level){
	port_pin_set_output_level(pin, level);
}

void pin_toggle(uint32_t pin){
	port_pin_toggle_output_level(pin);
}

bool pin_get(uint32_t pin){
	return port_pin_get_input_level(pin);
}