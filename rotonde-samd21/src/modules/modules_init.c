/*
 * modules_init.c
 *
 * Created: 20/01/2016 18:19:08
 *  Author: stant
 */ 

#include "modules/modules_init.h"

#include "modules/modules/template.h"

#include "modules/modules/pingpong.h"

void init_modules(void) {
	Pinpong_module_init();
}
