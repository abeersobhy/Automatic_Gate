/*
 * I2C.h
 *
 *  Created on: Nov 6, 2025
 *  Author: Abeer Sobhy
 */

#ifndef I2C_I2C_H_
#define I2C_I2C_H_

#include "gpio.h"
#include "RCC.h"

// @ref I2C_CLK_Speed
#define I2C_SCLK_SM_50K						(50000U)
#define I2C_SCLK_SM_100K					(100000U)
#define I2C_SCLK_FM_200K					(200000U)
#define I2C_SCLK_FM_400K					(400000U)

// @ref I2C_StretchMode Enable / Disable
#define I2C_StretchMode_Enable				0x00000000
#define I2C_StretchMode_Disable 			I2C_CR1_NOSTRETCH

// @ref I2C_Mode
#define I2C_Mode_I2C						0
#define I2C_Mode_SMBus						I2C_CR1_SMBUS

// @ref DualAddress /  Enable or Disable
#define DualAddress_Enable					I2C_OAR2_ENDUAL
#define DualAddress_Disable					0

// @ref Addressing_Slave_Mode
#define Addressing_Slave_Mode_7BIT			0
#define Addressing_Slave_Mode_10BIT 		uint16_t(1 << 15)

// @ref I2C_ACK_Control
#define I2C_ACK_Enable						I2C_CR1_ACK
#define I2C_ACK_Disable						0

// @ref I2C_GeneralCall_Adressing
#define I2C_ENGC_Enable						I2C_CR1_ENGC
#define I2C_ENGC_Disable					0

// @ref I2C_SM_OR_FM
#define I2C_SM_Enable						0
#define I2C_FM_Enable						(1 << 15)


#define I2C_Master_Enable					1
#define I2C_Slave_Enable					0

#define I2C_EVENT_MASTER_BYTE_TRANSMITTING	(uint32_t)0x00070080
typedef enum
{
	Start,
	RepeatedStart,
}RepeatedStart_e;

typedef enum
{
	Stop,
	NoStop,
}Stop_Condition_e;
typedef enum
{
	I2C_EV_STOP,
	I2C_ERROR_AF,
	I2C_EV_ADDR_Matched,
	I2C_EV_DATA_REQ,
	I2C_EV_DATA_REC,
}Slave_State;

typedef enum
{
	Start_Enable,
	Start_Disable,
}FunctionalState_e;

typedef struct
{
	uint16_t 		DualAddress;	//@ref DualAddress /  Enable or Disable
	uint16_t 		Slave_PrimaryAddress;
	uint16_t   		Slave_SecondaryAddress;
	uint16_t 		Addressing_Slave_Mode;		//@ref Addressing_Slave_Mode  7 bit or 10 bit

}I2C_Device_Address;


typedef struct
{
	uint32_t 		I2C_ClockSpeed;  		//  @ref I2C_CLK_Speed

	uint32_t 		I2C_StretchMode; 		// @ref I2C_StretchMode

	uint32_t 		I2C_Mode;		 		// @ref I2C_Mode

	I2C_Device_Address slaveAddressConfig;

	uint32_t 		I2C_ACK_Control; 		// I2C_ACK_Control - Automatically sent by hardware or None Ack

	uint32_t 		I2C_Master_Slave_Mode;	// Master or Slave

	uint32_t 		I2C_GeneralCall_Adressing; 	// @ref I2C_GeneralCall_Adressing enable or disable

	uint32_t 		I2C_SM_OR_FM;			// @ref I2C_SM_OR_FM

	void (*I2C_Slave_Event_CallBack)(Slave_State State);

	I2Cx_typedef* I2Cx;	// I2Cx where x can b 1 or 2

}I2C_Init_t;


void I2Cx_Init(I2C_Init_t* I2C_Init);

void I2C_Master_TX(I2Cx_typedef* I2Cx, const I2C_Device_Address* slave_address, uint8_t* dataOut, uint32_t dataLen, Stop_Condition_e st, RepeatedStart_e RS);

void I2C_Master_RX(I2Cx_typedef* I2Cx, const I2C_Device_Address* slave_address, uint8_t* dataOut, uint32_t dataLen, Stop_Condition_e st, RepeatedStart_e RS);

#endif /* I2C_I2C_H_ */
