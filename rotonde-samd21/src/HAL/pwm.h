/*
 * pwm.h
 *
 * Created: 31/05/2016 14:26:54
 *  Author: GroundLayer
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <asf.h>

void init_pwm(int HWtimer,int pin0, int mux0, int pin1, int mux1);
void set_pwm(int HWtimer, int channel, int value);

#endif /* PWM_H_ */