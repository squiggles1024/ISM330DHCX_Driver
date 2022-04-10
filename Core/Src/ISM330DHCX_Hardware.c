/*
 * ISM330DHCX_Hardware.c
 *
 *  Created on: Mar 16, 2022
 *      Author: evanl
 */

#include <ISM330DHCX_Hardware.h>
/**************************************//**************************************//**************************************
 * Includes
 **************************************//**************************************//**************************************/
#include "stm32u5xx_hal.h"
#include "i2c.h"
#include "gpio.h"
#include <stdint.h>
#include "log.h"

/**************************************//**************************************//**************************************
 * Defines / Constants
 **************************************//**************************************//**************************************/
/* Device Address */
#define ISM330DHCX_I2C_ADDR (0xD6U)

/**************************************//**************************************//**************************************
 * Private Function Prototypes
 **************************************//**************************************//**************************************/
static void ISM330DHCX_IO_Init();
static void ISM330DHCX_IO_DeInit();
static ISM330DHCX_Status_t ISM330DHCX_IO_WriteReg(uint8_t RegAddr, uint8_t *Data, uint8_t Length);
static ISM330DHCX_Status_t ISM330DHCX_IO_ReadReg(uint8_t RegAddr, uint8_t *Data, uint8_t Length);
static uint8_t ISM330DHCX_IO_IOCtl(ISM330DHCX_IO_Cmd_t Cmd);
static uint8_t ISM330DHCX_IO_ReadIntPin(uint16_t Pin);
static void ISM330DHCX_IO_IRQEnable(void);
static void ISM330DHCX_IO_IRQDisable(void);

/**************************************//**************************************//**************************************
 * Private Function Definitions
 **************************************//**************************************//**************************************/
static void ISM330DHCX_IO_Init(){
	  HAL_Delay(10);      /* Device has a 10ms boot sequence on power up */
	  ISM330DHCX_GPIO_Init();
	  MX_I2C2_Init();
}

static void ISM330DHCX_IO_DeInit(){
	  //HAL_I2C_DeInit(&hi2c2); Do not do this if more than one device shares the I2C bus
	  HAL_GPIO_DeInit(ISM330DHCX_IRQ_GPIO_Port, ISM330DHCX_IRQ_Pin); //De-init the GPIO Pin
}

static ISM330DHCX_Status_t ISM330DHCX_IO_WriteReg(uint8_t RegAddr, uint8_t *Data, uint8_t Length){
	if(HAL_I2C_Mem_Write(&hi2c2, ISM330DHCX_I2C_ADDR, RegAddr, I2C_MEMADD_SIZE_8BIT, Data, Length, HAL_MAX_DELAY) != HAL_OK){
		_log(log_i2c,"Write to ISM330DHCX Reg address %x failed.",RegAddr);
		return ISM330DHCX_ERROR;
	}
	return ISM330DHCX_OK;
}

static ISM330DHCX_Status_t ISM330DHCX_IO_ReadReg(uint8_t RegAddr, uint8_t *Data, uint8_t Length){
	if(HAL_I2C_Mem_Read(&hi2c2, ISM330DHCX_I2C_ADDR | 0x01, RegAddr, I2C_MEMADD_SIZE_8BIT, Data, Length, HAL_MAX_DELAY) != HAL_OK){
		_log(log_i2c,"Read from ISM330DHCX Reg address %x failed.",RegAddr);
		return ISM330DHCX_ERROR;
	}
	return ISM330DHCX_OK;
}

static uint8_t ISM330DHCX_IO_IOCtl(ISM330DHCX_IO_Cmd_t Cmd){
	uint8_t PinStatus;
	switch(Cmd){

	case ISM330DHCX_IRQEnable:
		ISM330DHCX_IO_IRQEnable();
		return ISM330DHCX_OK;
		break;

	case ISM330DHCX_IRQDisable:
		ISM330DHCX_IO_IRQDisable();
		return ISM330DHCX_OK;
		break;

	case ISM330DHCX_ReadInt1Pin:
		PinStatus = ISM330DHCX_IO_ReadIntPin(ISM330DHCX_IRQ_Pin);
		if(PinStatus == GPIO_PIN_SET){
			return 1;
		} else {
			return 0;
		}
	default:
		break;

	}
	return 0;
}

static uint8_t ISM330DHCX_IO_ReadIntPin(uint16_t Pin){
	if(HAL_GPIO_ReadPin(ISM330DHCX_IRQ_GPIO_Port, Pin) == GPIO_PIN_SET){
		return 1;
	} else {
		return 0;
	}
}

static void ISM330DHCX_IO_IRQEnable(void){
	NVIC_EnableIRQ(ISM330DHCX_IRQ_EXTI_IRQn);
}

static void ISM330DHCX_IO_IRQDisable(void){
	NVIC_DisableIRQ(ISM330DHCX_IRQ_EXTI_IRQn);
}

/**************************************//**************************************//**************************************
 * Public Variable Defitinion
 **************************************//**************************************//**************************************/
ISM330DHCX_IO_Drv_t ISM330DHCX_HardwareDrv = {
		.DeInit = ISM330DHCX_IO_DeInit,
		.Init = ISM330DHCX_IO_Init,
		.ReadReg = ISM330DHCX_IO_ReadReg,
		.WriteReg = ISM330DHCX_IO_WriteReg,
		.ioctl = ISM330DHCX_IO_IOCtl
};

