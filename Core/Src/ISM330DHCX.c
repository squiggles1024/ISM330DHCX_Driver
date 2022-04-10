/*
 * ISM330DHCX.c
 *
 *  Created on: Mar 17, 2022
 *      Author: evanl
 */

/**************************************//**************************************//**************************************
 * Includes
 **************************************//**************************************//**************************************/
#include <ISM330DHCX.h>
#include <stdio.h>
#include "stddef.h"
#include "log.h"
/**************************************//**************************************//**************************************
 * Public Function Definitions
 **************************************//**************************************//**************************************/
/********************************************************************************************************
 * Brief: Constructs and intializes an ISM330DHCX Device and links it to its low level drivers
 * Params: Settings: Desired user settings for device, Dev: Device to be initialized, ISM330DHCX_IO_Drv_t low level IO driver
 ********************************************************************************************************/
void ISM330DHCX_Init(ISM330DHCX_Init_Struct_t Settings, ISM330DHCX_Handle_t *Dev, ISM330DHCX_IO_Drv_t LowLevelDrivers){
	Dev->ISM330DHCX_IO.Init = LowLevelDrivers.Init;
	Dev->ISM330DHCX_IO.DeInit = LowLevelDrivers.DeInit;
	Dev->ISM330DHCX_IO.WriteReg = LowLevelDrivers.WriteReg;
	Dev->ISM330DHCX_IO.ReadReg = LowLevelDrivers.ReadReg;
	Dev->ISM330DHCX_IO.ioctl = LowLevelDrivers.ioctl;

	if(Settings.Int1 != 0 || Settings.Int2 != 0){
		Dev->ISM330DHCX_IO.ioctl(ISM330DHCX_IRQDisable);
	}

	Dev->ISM330DHCX_IO.Init();

	uint8_t DeviceID = 0;
	if(Dev->ISM330DHCX_IO.ReadReg(ISM330DHCX_REG_WHO_AM_I, &DeviceID, 1) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Initialization: Read Device ID Reg Failed.");
	}
	if(DeviceID != ISM330DHCX_DEVICE_ID){
		_log(log_ism330dhcx, "Initialization: Device ID Mismatch");
		return;
	}

	uint8_t settings_reg;

	if(Settings.Filter != LPF_Div2){

		/*Configure the low pass filter*/
		settings_reg = (Settings.Filter - 1) << 5;
		if(Dev->ISM330DHCX_IO.WriteReg(ISM330DHCX_REG_CTRL8_XL, &settings_reg, 1) != ISM330DHCX_OK){
			_log(log_ism330dhcx, "Initialization: Write Ctrl8 XL failed");
			return;
		}

		/*Configure the ODR and Sensitivity*/
		settings_reg = (Settings.AccelMode << 4) | (Settings.XL_Sense << 2) | (1 << 1);
		if(Dev->ISM330DHCX_IO.WriteReg(ISM330DHCX_REG_CTRL1_XL, &settings_reg, 1) != ISM330DHCX_OK){
			_log(log_ism330dhcx, "Initialization: Write Ctrl1 XL failed");
			return;
		}

	} else {

		/*Configure the ODR and Sensitivity*/
		settings_reg = (Settings.AccelMode << 4) | (Settings.XL_Sense << 2);
		if(Dev->ISM330DHCX_IO.WriteReg(ISM330DHCX_REG_CTRL1_XL, &settings_reg, 1) != ISM330DHCX_OK){
			_log(log_ism330dhcx, "Initialization: Write Ctrl1 XL failed");
			return;
		}
	}

	/*
	if(Dev->ISM330DHCX_IO.WriteReg(ISM330DHCX_REG_CTRL1_XL, &settings_reg, 1) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Initialization: Write Ctrl1 XL failed");
		return;
	}
	*/


	if(Dev->ISM330DHCX_IO.WriteReg(ISM330DHCX_REG_INT1_CTRL, &Settings.Int1, 1) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Initialization: Write INT1 Ctrl failed");
		return;
	}

	settings_reg = (Settings.GyroMode << 4) | (Settings.G_Sense << 2);
	if(Dev->ISM330DHCX_IO.WriteReg(ISM330DHCX_REG_CTRL2_G, &settings_reg, 1) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Initialization: Write Ctrl2 reg failed");
		return;
	}

	if(Dev->ISM330DHCX_IO.WriteReg(ISM330DHCX_REG_INT2_CTRL, &Settings.Int2, 1) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Initialization: Write Int2 ctrl reg failed");
		return;
	}

	Dev->G_Sensitivity = Settings.G_Sense;
	Dev->XL_Sensitivity = Settings.XL_Sense;
	if(Settings.Int1 == 0 && Settings.Int2 == 0){
		return;
	}

	if(Settings.Int1 != 0 || Settings.Int2 != 0){
		Dev->ISM330DHCX_IO.ioctl(ISM330DHCX_IRQEnable);
	}

	/*Ensure IRQ Line is cleared*/
	ISM330DHCX_ReadAcceleration(Dev);
	ISM330DHCX_ReadAngularVelocity(Dev);
}


/********************************************************************************************************
 * Brief: Reads Acceleration registers on ISM330DHCX and stores them in the device handle variables
 * Params: ISM330DHCX Device Handle
 ********************************************************************************************************/
ISM330DHCX_DataReadyStatus_t ISM330DHCX_ReadAcceleration(ISM330DHCX_Handle_t *Dev){
	int16_t *pData = Dev->Acceleration;
	uint8_t status = 0;
	if(Dev->ISM330DHCX_IO.ReadReg(ISM330DHCX_REG_STATUS_REG,&status,1) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Read Status reg failed");
		return ISM330DHCX_DataNotReady;
	} else if((status & 0x01) != 0x01){
		Dev->DataReadyFlag = ISM330DHCX_DataNotReady;
		return ISM330DHCX_DataNotReady;
	}
	Dev->DataReadyFlag = ISM330DHCX_DataReady;
	if (Dev->ISM330DHCX_IO.ReadReg(ISM330DHCX_REG_OUTX_L_A, (uint8_t*)pData, 6) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Read Acceleration failed");
	}
	Dev->DataReadyFlag = ISM330DHCX_DataNotReady;
	return ISM330DHCX_DataReady;
}

/********************************************************************************************************
 * Brief: Reads Gyro registers on ISM330DHCX and stores them in the device handle variables
 * Params: ISM330DHCX Device Handle
 ********************************************************************************************************/
ISM330DHCX_DataReadyStatus_t ISM330DHCX_ReadAngularVelocity(ISM330DHCX_Handle_t *Dev){
	int16_t *pData = Dev->AngularVelocity;
	uint8_t status = 0;
	if(Dev->ISM330DHCX_IO.ReadReg(ISM330DHCX_REG_STATUS_REG,&status,1) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Read Status reg failed");
		return ISM330DHCX_DataNotReady;
	} else if((status & 0x02) != 0x02){
		Dev->DataReadyFlag = ISM330DHCX_DataNotReady;
		return ISM330DHCX_DataNotReady;
	}
	Dev->DataReadyFlag = ISM330DHCX_DataReady;
	if (Dev->ISM330DHCX_IO.ReadReg(ISM330DHCX_REG_OUTX_L_G, (uint8_t*)pData, 6) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Read Acceleration failed");
	}
	Dev->DataReadyFlag = ISM330DHCX_DataNotReady;
	return ISM330DHCX_DataReady;
}


/********************************************************************************************************
 * Brief: Resets Device and de-intializes low level IO
 * Params: ISM330DHCX Device Handle
 ********************************************************************************************************/
void ISM330DHCX_DeInit(ISM330DHCX_Handle_t *Dev){
	ISM330DHCX_Reset(Dev);
	Dev->ISM330DHCX_IO.DeInit();
	Dev->ISM330DHCX_IO.DeInit = NULL;
	Dev->ISM330DHCX_IO.Init = NULL;
	Dev->ISM330DHCX_IO.WriteReg = NULL;
	Dev->ISM330DHCX_IO.ReadReg = NULL;
	Dev->ISM330DHCX_IO.WriteReg = NULL;
}


/********************************************************************************************************
 * Brief: Resets Device to factory default settings
 * Params: ISM330DHCX Device Handle
 ********************************************************************************************************/
void ISM330DHCX_Reset(ISM330DHCX_Handle_t *Dev){
	uint8_t reset_signal = 1;
	if(Dev->ISM330DHCX_IO.WriteReg(ISM330DHCX_REG_CTRL3_C, &reset_signal, 1) != ISM330DHCX_OK){ //Writing a 1 to this register resets the device
		_log(log_ism330dhcx, "Reset failed");
	}
}

/********************************************************************************************************
 * Brief: Reads a specific register from device
 * Params: ISM330DHCX Device Handle, register to read, data buffer, amount of bytes to read
 ********************************************************************************************************/
void ISM330DHCX_ReadReg(ISM330DHCX_Handle_t *Dev, uint8_t reg, uint8_t *pdata, uint8_t length){
	if(Dev->ISM330DHCX_IO.ReadReg(reg,pdata,length) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Read Reg %x failed.",reg);
	}
}

/********************************************************************************************************
 * Brief: Writes to a specific register on device
 * Params: ISM330DHCX Device Handle, register to write to, data buffer, amount of bytes to write
 ********************************************************************************************************/
void ISM330DHCX_WriteReg(ISM330DHCX_Handle_t *Dev, uint8_t reg, uint8_t *pdata, uint8_t length){
	if(Dev->ISM330DHCX_IO.WriteReg(reg,pdata,length) != ISM330DHCX_OK){
		_log(log_ism330dhcx, "Write Reg %x failed.",reg);
	}
}

