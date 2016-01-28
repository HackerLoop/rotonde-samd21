/**
******************************************************************************
* @file       crc.h
* @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
* @brief      Include file of the crc checker
* @brief
*
*****************************************************************************/



#ifndef CRC_H_
#define CRC_H_

#include "asf.h"

uint8_t CRC_updateByte(uint8_t crc, const uint8_t data);
uint8_t CRC_updateCRC(uint8_t crc, const uint8_t* data, int32_t length);

#endif /* CRC_H_ */