/*
 * ISM330DHCX.h
 *
 *  Created on: Mar 16, 2022
 *      Author: evanl
 */

#ifndef INC_ISM330DHCX_H_
#define INC_ISM330DHCX_H_
/**************************************//**************************************//**************************************
 * Includes
 **************************************//**************************************//**************************************/
#include <ISM330DHCX_Hardware.h> //Low level IO dependency

/**************************************//**************************************//**************************************
 * Typedefs / Enumerations
 **************************************//**************************************//**************************************/
typedef enum{
	ISM330DHCX_DataReady,
	ISM330DHCX_DataNotReady
}ISM330DHCX_DataReadyStatus_t;


/* Accelerometer output data rate */
typedef enum{
	XL_Power_Down,
	XL_LowPower_13Hz,
	XL_LowPower_26Hz,
	XL_LowPower_52Hz,
	XL_NormalMode_104Hz,
	XL_NormalMode_208Hz,
	XL_HighPerf_416Hz,
	XL_HighPerf_833Hz,
	XL_HighPerf_1660Hz,
	XL_HighPerf_3330Hz,
	XL_HighPerf_6660Hz,
	XL_LowPower_2Hz
}ISM330DHCX_AccelerometerMode_t;

/* Gyro output data rate */
typedef enum{
	G_Power_Down,
	G_LowPower_13Hz,
	G_LowPower_26Hz,
	G_LowPower_52Hz,
	G_NormalMode_104Hz,
	G_NormalMode_208Hz,
	G_HighPerf_416Hz,
	G_HighPerf_833Hz,
	G_HighPerf_1660Hz,
	G_HighPerf_3330Hz,
	G_HighPerf_6660Hz,
	G_LowPower_2Hz
}ISM330DHCX_GyroscopeMode_t;

/*IRQ = Low-to-High polarity by default*/
typedef enum{
	INT1_StepDetect = 1 << 7,
	INT1_SignificantMotion = 1 << 6,
	INT1_FifoFull= 1 << 5,
	INT1_FifoOverrun= 1 << 4,
	INT1_FifoThreshold = 1 << 3,
	INT1_BootStatus = 1 << 2,
	INT1_GDataRdy = 1 << 1,
	INT1_XDataRdy = 1 << 0,
	INT1_Disable = 0
}ISM330DHCX_INT1_IRQMode_t;

typedef enum{
	INT2_StepDelta = 1 << 7,
	INT2_StepOverflow = 1 << 6,
	INT2_FifoFull = 1 << 5,
	INT2_FifoOverrun = 1 << 4,
	INT2_FifoThreshold = 1 << 3,
	INT2_TDataRdy = 1 << 2,
	INT2_XDataRdy = 1 << 1,
	INT2_GDataRdy = 1 << 0,
	INT2_Disable = 0
}ISM330DHCX_INT2_IRQMode_t;


/* PM = plus or minus. e.g. Plus or Minus 16g. This effects the int16->mg conversion ratio and fullscale measurements*/
typedef enum{
	PM2G = 0,
	PM16G,
	PM4G,
	PM8G
}ISM330DHCX_AccelSensitivity_t;

/*A 125 DPS Mode is also available. See Datasheet for details*/
typedef enum{
	PM250DPS = 0,
	PM500DPS,
	PM1000DPS,
	PM2000DPS
}ISM330DHCX_GyroSensitivity_t;

/* Corner frequency of internal low pass filter for accelerometer */
typedef enum{
	LPF_Div2,
	LPF_Div4,
	LPF_Div10,
	LPF_Div20,
	LPF_Div45,
	LPF_Div100,
	LPF_Div200,
	LFP_Div400,
	LPF_Div800
}ISM330DHCX_AccelFilterMode_t;

/**************************************//**************************************//**************************************
 * Driver Structs
 **************************************//**************************************//**************************************/
/*User must provide one of these structs to the Init function*/
typedef struct{
	ISM330DHCX_AccelerometerMode_t AccelMode;
	ISM330DHCX_AccelerometerMode_t GyroMode;
	ISM330DHCX_AccelSensitivity_t XL_Sense;
	ISM330DHCX_GyroSensitivity_t G_Sense;
	ISM330DHCX_INT1_IRQMode_t Int1; /* Bitwise OR All desired IRQs together */
	ISM330DHCX_INT2_IRQMode_t Int2; /* Bitwise OR All desired IRQs together */
	ISM330DHCX_AccelFilterMode_t Filter;
}ISM330DHCX_Init_Struct_t;

/*User must provide the low level driver: ISM330DHCX_IO_Drv_t. See ISM330DHCX_Hardware.h for details */
typedef struct{
	int16_t AngularVelocity[3];
	int16_t Acceleration[3];
	float XL_Sensitivity;
	float G_Sensitivity;
	ISM330DHCX_DataReadyStatus_t DataReadyFlag;
	ISM330DHCX_IO_Drv_t ISM330DHCX_IO;
}ISM330DHCX_Handle_t;


/**************************************//**************************************//**************************************
 * Defines
 **************************************//**************************************//**************************************/
/* Device ID (See "WHO_AM_I" reg) */
#define ISM330DHCX_DEVICE_ID (0x6BU)

/* Embedded functions configuration register */
#define ISM330DHCX_REG_FUNC_CFG_ACCESS (0x01U)
#define ISM330DHCX_REG_PIN_CTRL (0x02)

/* Embedded functions configuration register */
#define ISM330DHCX_REG_FIFO_CTRL1 (0x07U)
#define ISM330DHCX_REG_FIFO_CTRL2 (0x08U)
#define ISM330DHCX_REG_FIFO_CTRL3 (0x09U)
#define ISM330DHCX_REG_FIFO_CTRL4 (0x0AU)

/* Counter batch data rate register */
#define ISM330DHCX_REG_COUNTER_BDR_REG1 (0x0BU)
#define ISM330DHCX_REG_COUNTER_BDR_REG2 (0x0CU)

/*  INT1 pin control */
#define ISM330DHCX_REG_INT1_CTRL (0x0DU)

/*  INT2 pin control */
#define ISM330DHCX_REG_INT2_CTRL (0x0EU)

/*  Who I am ID */
#define ISM330DHCX_REG_WHO_AM_I (0x0FU)

/* Accelerometer and gyroscope control registers */
#define ISM330DHCX_REG_CTRL1_XL (0x10U)
#define ISM330DHCX_REG_CTRL2_G (0x11U)
#define ISM330DHCX_REG_CTRL3_C (0x12U)
#define ISM330DHCX_REG_CTRL4_C (0x13U)
#define ISM330DHCX_REG_CTRL5_C (0x14U)
#define ISM330DHCX_REG_CTRL6_C (0x15U)
#define ISM330DHCX_REG_CTRL7_G (0x16U)
#define ISM330DHCX_REG_CTRL8_XL (0x17U)
#define ISM330DHCX_REG_CTRL9_XL (0x18U)
#define ISM330DHCX_REG_CTRL10_C (0x19U)

/* Interrupt registers */
#define ISM330DHCX_REG_ALL_INT_SRC (0x1AU)
#define ISM330DHCX_REG_WAKE_UP_SRC (0x1BU)
#define ISM330DHCX_REG_TAP_SRC (0x1CU)
#define ISM330DHCX_REG_D6D_SRC (0x1DU)

/* Status data register for user interface */
#define ISM330DHCX_REG_STATUS_REG (0x1EU)

/* Temperature output data registers */
#define ISM330DHCX_REG_OUT_TEMP_L (0x20U)
#define ISM330DHCX_REG_OUT_TEMP_H (0x21U)

/* Gyroscope output registers for user interface */
#define ISM330DHCX_REG_OUTX_L_G (0x22U)
#define ISM330DHCX_REG_OUTX_H_G (0x23U)
#define ISM330DHCX_REG_OUTY_L_G (0x24U)
#define ISM330DHCX_REG_OUTY_H_G (0x25U)
#define ISM330DHCX_REG_OUTZ_L_G (0x26U)
#define ISM330DHCX_REG_OUTZ_H_G (0x27U)

/* Accelerometer output registers */
#define ISM330DHCX_REG_OUTX_L_A (0x28U)
#define ISM330DHCX_REG_OUTX_H_A (0x29U)
#define ISM330DHCX_REG_OUTY_L_A (0x2AU)
#define ISM330DHCX_REG_OUTY_H_A (0x2BU)
#define ISM330DHCX_REG_OUTZ_L_A (0x2CU)
#define ISM330DHCX_REG_OUTZ_H_A (0x2DU)

/* FSM Registers */
#define ISM330DHCX_REG_EMB_FUNC_STATUS_MAINPAGE (0x35U)
#define ISM330DHCX_REG_FSM_STATUS_A_MAINPAGE (0x36U)
#define ISM330DHCX_REG_FSM_STATUS_B_MAINPAGE (0x37U)

/* Machine Learning Core Register */
#define ISM330DHCX_REG_MLC_STATUS_MAINPAGE (0x38U)

/* Sensor Hub source register */
#define ISM330DHCX_REG_STATUS_MASTER_MAINPAGE (0x39U)

/* FIFO status registers */
#define ISM330DHCX_REG_FIFO_STATUS1 (0x3AU)
#define ISM330DHCX_REG_FIFO_STATUS2 (0x3BU)

/* Timestamp output registers */
#define ISM330DHCX_REG_TIMESTAMP0_REG (0x40U)
#define ISM330DHCX_REG_TIMESTAMP1_REG (0x41U)
#define ISM330DHCX_REG_TIMESTAMP2_REG (0x42U)
#define ISM330DHCX_REG_TIMESTAMP3_REG (0x43U)

/* Interrupt registers */
#define ISM330DHCX_REG_TAP_CFG0 (0x56U)
#define ISM330DHCX_REG_TAP_CFG1 (0x57U)
#define ISM330DHCX_REG_TAP_CFG2 (0x58U)
#define ISM330DHCX_REG_TAP_THS_6D (0x59U)
#define ISM330DHCX_REG_INT_DUR2 (0x5AU)
#define ISM330DHCX_REG_WAKE_UP_THS (0x5BU)
#define ISM330DHCX_REG_WAKE_UP_DUR (0x5CU)
#define ISM330DHCX_REG_FREE_FALL (0x5DU)
#define ISM330DHCX_REG_MD1_CFG (0x5EU)
#define ISM330DHCX_REG_MD2_CFG (0x5FU)

/* Internal frequency register */
#define ISM330DHCX_REG_INTERNAL_FREQ_FINE (0x63U)

/* Optical Image Stabilization Registers */
#define ISM330DHCX_REG_INT_OIS (0x6FU)
#define ISM330DHCX_REG_CTRL1_OIS (0x70U)
#define ISM330DHCX_REG_CTRL2_OIS (0x71U)
#define ISM330DHCX_REG_CTRL3_OIS (0x72U)

/* Accelerometer user offset correction */
#define ISM330DHCX_REG_X_OFS_USR (0x73U)
#define ISM330DHCX_REG_Y_OFS_USR (0x74U)
#define ISM330DHCX_REG_Z_OFS_USR (0x75U)

/* Fifo data out registers, Tag = Sensor which produced the data in FIFO */
#define ISM330DHCX_REG_FIFO_DATA_OUT_TAG (0x78U)
#define ISM330DHCX_REG_FIFO_OUT_X_L (0x79U)
#define ISM330DHCX_REG_FIFO_OUT_X_H (0x7AU)
#define ISM330DHCX_REG_FIFO_OUT_Y_L (0x7BU)
#define ISM330DHCX_REG_FIFO_OUT_Y_H (0x7CU)
#define ISM330DHCX_REG_FIFO_OUT_Z_L (0x7DU)
#define ISM330DHCX_REG_FIFO_OUT_Z_H (0x7EU)


#define XL_READING_TO_MG(x)	((x)*.061 + .061) //See Mechanical Characteristics of data sheet (.061, .122,.244, .488)
#define G_READING_TO_MDPS(x) ((x)*4.375 + 4.375)

/***********************************************************************/
/* Public Function Declarations */
/**********************************************************************/
void ISM330DHCX_Init(ISM330DHCX_Init_Struct_t Settings, ISM330DHCX_Handle_t *Dev, ISM330DHCX_IO_Drv_t LowLevelDrivers);
void ISM330DHCX_DeInit(ISM330DHCX_Handle_t *Dev);
void ISM330DHCX_Reset(ISM330DHCX_Handle_t *Dev);
ISM330DHCX_DataReadyStatus_t ISM330DHCX_ReadAcceleration(ISM330DHCX_Handle_t *Dev);
ISM330DHCX_DataReadyStatus_t ISM330DHCX_ReadAngularVelocity(ISM330DHCX_Handle_t *Dev);
void ISM330DHCX_ReadReg(ISM330DHCX_Handle_t *Dev, uint8_t reg, uint8_t *pdata, uint8_t length);
void ISM330DHCX_WriteReg(ISM330DHCX_Handle_t *Dev, uint8_t reg, uint8_t *pdata, uint8_t length);


#endif /* INC_ISM330DHCX_H_ */
