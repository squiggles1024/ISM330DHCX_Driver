/*
 * ISM330DHCX_Hardware.h
 *
 *  Created on: Mar 16, 2022
 *      Author: evanl
 */

#ifndef INC_ISM330DHCX_HARDWARE_H_
#define INC_ISM330DHCX_HARDWARE_H_
/**************************************************
 * Includes
 **************************************************/
#include <stdint.h>

/**************************************************
 * Typedefs
 **************************************************/
typedef enum{
	ISM330DHCX_OK = 0,
	ISM330DHCX_ERROR
} ISM330DHCX_Status_t;

/*Additional low level IO driver commands*/
typedef enum{
	IRQEnable = 0,
	IRQDisable,
	ReadInt1Pin,
	ReadInt2Pin,
}ISM330DHCX_IO_Cmd_t;


/**************************************************
 * Low Level Driver
 **************************************************/

typedef struct{
	void (*Init)(); //Performs Low level IO initialization (e.g. GPIO, I2C or SPI)
	void (*DeInit)(); //De initializes Low Level IO drivers
	ISM330DHCX_Status_t (*WriteReg)(uint8_t, uint8_t*, uint8_t); //Params: Reg Addr, Data, Length
	ISM330DHCX_Status_t (*ReadReg)(uint8_t, uint8_t*, uint8_t);  //Params: Reg Addr, Data Buffer, Length
	uint8_t (*ioctl)(ISM330DHCX_IO_Cmd_t); //Other desired functions (e.g. read INT pin, disable IRQ, etc.)
}ISM330DHCX_IO_Drv_t;

/**************************************************
 * Exported Variable - User may or may not choose to use this.
 **************************************************/
extern ISM330DHCX_IO_Drv_t ISM330DHCX_HardwareDrv;


#endif /* INC_ISM330DHCX_HARDWARE_H_ */
