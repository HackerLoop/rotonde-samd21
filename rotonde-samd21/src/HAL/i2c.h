/*
 * HAL_I2C.h
 *
 * Created: 13/01/2016 12:13:04
 *  Author: GroundLayer
 */ 


#ifndef HAL_I2C_H_
#define HAL_I2C_H_

#include "asf.h"

struct i2c_master_module i2c_master_instance;

enum status_code HAL_I2C_init(void);
enum status_code HAL_I2C_write(unsigned char slave_addr,unsigned char length, unsigned char *data);
enum status_code HAL_I2C_read(unsigned char slave_addr,unsigned char length, unsigned char *data);



#endif /* HAL_I2C_H_ */