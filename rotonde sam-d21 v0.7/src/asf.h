/**
 * \file
 *
 * \brief Autogenerated API include file for the Atmel Software Framework (ASF)
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef ASF_H
#define ASF_H

/*
 * This file includes all API header files for the selected drivers from ASF.
 * Note: There might be duplicate includes required by more than one driver.
 *
 * The file is automatically generated and will be re-written when
 * running the ASF driver selector tool. Any changes will be discarded.
 */

// From module: Common SAM0 compiler driver
#include <compiler.h>
#include <status_codes.h>

// From module: Common build items for user board support templates
#include <user_board.h>

// From module: EXTINT - External Interrupt (Callback APIs)
#include <extint.h>
#include <extint_callback.h>

// From module: FreeRTOS - kernel 8.0.1
#include <FreeRTOS.h>
#include <StackMacros.h>
#include <croutine.h>
#include <event_groups.h>
#include <list.h>
#include <mpu_wrappers.h>
#include <portable.h>
#include <projdefs.h>
#include <queue.h>
#include <semphr.h>
#include <task.h>
#include <timers.h>

// From module: Generic board support
#include <board.h>

// From module: Interrupt management - SAM implementation
#include <interrupt.h>

// From module: PORT - GPIO Pin Control
#include <port.h>

// From module: Part identification macros
#include <parts.h>

// From module: SAM D21/R21/D11/L21/DA1/L22 USB Dual role interface
#include <usb_dual.h>

// From module: SYSTEM - Clock Management for SAMD21/R21/DAx
#include <clock.h>
#include <gclk.h>

// From module: SYSTEM - Core System Driver
#include <system.h>

// From module: SYSTEM - I/O Pin Multiplexer
#include <pinmux.h>

// From module: SYSTEM - Interrupt Driver
#include <system_interrupt.h>

// From module: SYSTEM - Power Management for SAM D20/D21/R21/D09/D10/D11/DA0/DA1
#include <power.h>

// From module: SYSTEM - Reset Management for SAM D20/D21/R21/D09/D10/D11/DA0/DA1
#include <reset.h>

// From module: Sleep manager - SAMD implementation
#include <samd/sleepmgr.h>
#include <sleepmgr.h>

// From module: USB - Universal Serial Bus
#include <usb.h>

// From module: USB Device HID Generic (Single Interface Device)
#include <udi_hid_generic.h>

// From module: USB Device Stack Core (Common API)
#include <udc.h>
#include <udd.h>

// From module: USB HID Device protocol
#include <usb_protocol_hid.h>

// From module: USB HID Library (Device)
#include <udi_hid.h>

#endif // ASF_H
