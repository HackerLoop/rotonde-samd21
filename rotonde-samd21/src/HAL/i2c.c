/*
 * HAL_I2C.c
 *
 * Created: 13/01/2016 12:07:05
 *  Author: GroundLayer
 */ 

#include "HAL/i2c.h"

/**
 * \brief init I2C peripheral
 *
 * \param[out]  status_code		status code of the operation
 */
enum status_code HAL_I2C_init(void){
	enum status_code result;
	
	/* Initialize config structure and software module. */
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	
	/* Change buffer timeout to something longer. */
	config_i2c_master.buffer_timeout = 10000;

	/* Initialize and enable device with config. */
	result =  i2c_master_init(&i2c_master_instance, SERCOM2, &config_i2c_master);
	
	//! [enable_module]
	i2c_master_enable(&i2c_master_instance);
	
	return result;
}

/**
 * \brief write a set of data on the I2C
 *
 *
 * \param[in]  slave_addr		address of slave to write to
 * \param[in]  length			length of the data to write
 * \param[in]  data				pointer to data to write
 * \param[out]  status_code		status code of the operation
 */
enum status_code HAL_I2C_write(unsigned char slave_addr,unsigned char length, unsigned char *data){
	
	struct i2c_master_packet packet = {
		.address     = slave_addr,
		.data_length = length,
		.data        = (uint8_t *) data,
		.ten_bit_address = false,
		.high_speed      = false,
		.hs_master_code  = 0x0,
	};
	
	return i2c_master_write_packet_wait(&i2c_master_instance, &packet);
	
}

/**
 * \brief read a set of data on the I2C
 *
 *
 * \param[in]  slave_addr		address of slave to read from
 * \param[in]  length			length of the data to read
 * \param[in]  data				pointer to data read
 * \param[out] status_code		status code of the operation
 */
enum status_code HAL_I2C_read(unsigned char slave_addr,unsigned char length, unsigned char *data){
	
	struct i2c_master_packet packet = {
		.address     = slave_addr,
		.data_length = length,
		.data        = (uint8_t *) data,
		.ten_bit_address = false,
		.high_speed      = false,
		.hs_master_code  = 0x0,
	};
	
	return i2c_master_read_packet_wait(&i2c_master_instance, &packet);
	
}
