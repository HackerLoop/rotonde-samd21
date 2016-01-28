# Rotonde-SAMD21
A Json over HID device to painlessly interface hardware with software on rotonde (based on an Atmel SAMD21), Prototyped on a SAMD21 Xplained Pro

## Project configuration
(All config files used listed in ../Configs)

### Atmel Studio asf wizard configuration

- Generic board support (driver)
- Debug print (FreeRTOS) (service) [to use the usb debug port as console]
- USB Device (service) : hid_generic
- EXTINT -External Interrupt (driver) : callback
- PORT -GPIO Pin Control (driver)
- SYSTEM -Core System Driver (driver)
- FreeRTOS mini Real-Time Kernel (service) : 8.0.1

- SERCOM USART - Serial Communications (driver) : callback
- 
ADD cpnf board

###Clock configurations

Configure system clock bus :
```
/* System clock bus configuration */
#  define CONF_CLOCK_CPU_CLOCK_FAILURE_DETECT     false
#  define CONF_CLOCK_FLASH_WAIT_STATES            2
#  define CONF_CLOCK_CPU_DIVIDER                  SYSTEM_MAIN_CLOCK_DIV_1
#  define CONF_CLOCK_APBA_DIVIDER                 SYSTEM_MAIN_CLOCK_DIV_1
#  define CONF_CLOCK_APBB_DIVIDER                 SYSTEM_MAIN_CLOCK_DIV_1
#  define CONF_CLOCK_APBC_DIVIDER                 SYSTEM_MAIN_CLOCK_DIV_1
```

Config DFLL for USB operations :
```
/* SYSTEM_CLOCK_SOURCE_DFLL configuration - Digital Frequency Locked Loop */
#  define CONF_CLOCK_DFLL_ENABLE                  true
#  define CONF_CLOCK_DFLL_LOOP_MODE               SYSTEM_CLOCK_DFLL_LOOP_MODE_USB_RECOVERY
#  define CONF_CLOCK_DFLL_ON_DEMAND               true
```

Set the main clock to run in standby :
```
/* Configure GCLK generator 0 (Main Clock) */
#  define CONF_CLOCK_GCLK_0_ENABLE                true
#  define CONF_CLOCK_GCLK_0_RUN_IN_STANDBY        true
#  define CONF_CLOCK_GCLK_0_CLOCK_SOURCE          SYSTEM_CLOCK_SOURCE_OSC8M
#  define CONF_CLOCK_GCLK_0_PRESCALER             1
#  define CONF_CLOCK_GCLK_0_OUTPUT_ENABLE         false
```
Config Clock Generator to use DFLL to clock USB :
```
/* Configure GCLK generator 3 */
#  define CONF_CLOCK_GCLK_3_ENABLE                true
#  define CONF_CLOCK_GCLK_3_RUN_IN_STANDBY        true
#  define CONF_CLOCK_GCLK_3_CLOCK_SOURCE          SYSTEM_CLOCK_SOURCE_DFLL
#  define CONF_CLOCK_GCLK_3_PRESCALER             1
#  define CONF_CLOCK_GCLK_3_OUTPUT_ENABLE         false
```

###DBG-Printer configuration
Configure the FreeRTOS debug printer to use with the embedded debuger module (USB port used to flash the board)
