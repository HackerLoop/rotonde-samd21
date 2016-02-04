/*
 * interrupt.h
 *
 * Created: 04/02/2016 16:05:07
 *  Author: GroundLayer
 */ 


#ifndef DIGITAL_PINS_H
#define DIGITAL_PINS_H

#include "asf.h"

void pinmode_in(uint32_t pinn, enum port_pin_pull input_pull);
void attach_interrupt(uint32_t pin, enum port_pin_pull input_pull,enum extint_detect detection_criteria,bool filter_input_signal, uint8_t eic, void (*callback)(void));
void pinmode_out(uint32_t pin);
void pin_set(uint32_t pin, bool level);
bool pin_get(uint32_t pin);
void pin_toggle(uint32_t pin);

#endif /* DIGITAL_PINS_H */