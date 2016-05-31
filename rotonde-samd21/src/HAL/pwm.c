/*
* pwm.c
*
* Created: 31/05/2016 14:26:47
*  Author: GroundLayer
*/

#include "pwm.h"

struct tcc_module tcc_instance[5];
Tcc *const hw[] = {TCC0 , TCC1, TCC2};

void init_pwm(int HWtimer,int pin0, int mux0, int pin1, int mux1){
	
	struct tcc_config config_tcc;
	tcc_get_config_defaults(&config_tcc, hw[HWtimer]);
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	config_tcc.counter.period = 0xFFFF;

	if(pin0 != 0 && mux0 != 0){
		config_tcc.pins.enable_wave_out_pin[0] = true;
		config_tcc.pins.wave_out_pin[0] = pin0;
		config_tcc.pins.wave_out_pin_mux[0]  = mux0;
	}
	if(pin1 != 0 && mux0 != 0){
		config_tcc.pins.enable_wave_out_pin[1] = true;
		config_tcc.pins.wave_out_pin[1] = pin1;
		config_tcc.pins.wave_out_pin_mux[1]  = mux1;
	}

	tcc_init(&tcc_instance[HWtimer],  hw[HWtimer], &config_tcc);
	tcc_enable(&tcc_instance[HWtimer]);
}

void set_pwm(int HWtimer, int channel, int value){
	tcc_set_compare_value(&tcc_instance[HWtimer], channel, value);
}