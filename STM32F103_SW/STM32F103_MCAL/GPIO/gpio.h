/*
 * gpio.h
 *
 *  Created on: Oct 2, 2025
 *      Author: A&M
 */

#ifndef GPIO_H_
#define GPIO_H_

/********************************************
 * Includes
*********************************************/
#include "stm32f103xx.h"

/********************************************
 * User typedef (structures)
*********************************************/

typedef struct
{
	uint16_t GPIO_PinNumber;
	uint8_t  GPIO_PinMode;
	uint8_t  GPIO_PinSpeed;

}GPIO_Pin_Config;

/********************************************
 * Macros Configurations
*********************************************/
// @ref Pin_numbers
#define GPIOx_PIN_0				(0)
#define GPIOx_PIN_1				(1)
#define GPIOx_PIN_2				(2)
#define GPIOx_PIN_3				(3)
#define GPIOx_PIN_4				(4)
#define GPIOx_PIN_5				(5)
#define GPIOx_PIN_6				(6)
#define GPIOx_PIN_7				(7)
#define GPIOx_PIN_8				(8)
#define GPIOx_PIN_9				(9)
#define GPIOx_PIN_10			(10)
#define GPIOx_PIN_11			(11)
#define GPIOx_PIN_12			(12)
#define GPIOx_PIN_13			(13)
#define GPIOx_PIN_14			(14)
#define GPIOx_PIN_15			(15)
#define GPIOx_PIN_ALL			(16)

// @ref Pin_mode

#define GPIO_MODE_ANALOG			 0x00000000u
#define GPIO_MODE_INPUT_FLOATING	 0x00000001u
#define GPIO_MODE_INPUT_PU			 0x00000002u
#define GPIO_MODE_INPUT_PD           0x00000003u
#define GPIO_MODE_OUTPUT_PP          0x00000004u
#define GPIO_MODE_OUTPUT_OD          0x00000005u
#define GPIO_MODE_AFIO_PP            0x00000006u
#define GPIO_MODE_AFIO_OD            0x00000007u
#define GPIO_MODE_AFIO_IN            0x00000008u

// @ref speed

#define GPIO_SPEED_10MHZ			((uint8_t)0x01)
#define GPIO_SPEED_2MHZ				((uint8_t)0x02)
#define GPIO_SPEED_50MHZ			((uint8_t)0x03)

//Pin statues
#define TRUE						1
#define FALSE						0

//LOCK STATUES
#define GPIO_RETURN_LOCK_Enabled    1
#define GPIO_RETURN_LOCK_ERROR		0


#define SET_BIT(x,b) 				((x) |= (1 << b))
#define CLR_BIT(x,b) 				((x) &= ~(1 << b))
#define TOGGLE_BIT(x,b) 			((x) ^= (1 << b))
#define GET_BIT(x,b) 				(((x) >> b) & 1)


/********************************************
 * Enums Configurations
*********************************************/

/********************************************
 * APIs supported by the GPIO driver
 * uint8_t MCAL_GPIO_ReadPin
 * uint16_t MCAL_GPIO_ReadPort
 * void MCAL_GPIO_WritePin
 * void MCAL_GPIO_WritePort
 * void MCAL_GPIO_TogglePin
 * void MCAL_GPIO_Init
 * void MCAL_GPIO_DeInit
 * void MCAL_GPIO_EXTI_IRQConfig
 * void MCAL_GPIO_EXTI_IRQHandling
 * void MCAL_GPIO_LockPin
*********************************************/

void MCAL_GPIO_Init(GPIOx_typedef* GPIOx , GPIO_Pin_Config* pinConfig);
/*================================================
 * @Fn				-MCAL_GPIO_DeInit
 * @breif			-DeInitialize(Reset) the selected GPIOx port
 * @param			-GPIOx where x can be A, B, or C port
 * @retVal			-none
 *================================================*/
void MCAL_GPIO_DeInit(GPIOx_typedef* GPIOx);
/*================================================
 * @Fn				-MCAL_GPIO_ReadPin
 * @breif			-Read specific PIN
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-pinNumber can be one of reference @ref Pin_numbers
 * @retVal			-uint8_t value refers to the state of the pin
 *================================================*/
uint8_t MCAL_GPIO_ReadPin(GPIOx_typedef* GPIOx , uint16_t pinNumber);
/*================================================
 * @Fn				-MCAL_GPIO_WritePin
 * @breif			-Read specific PIN
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-pinNumber can be one of reference @ref Pin_numbers
 * @param			-uint8_t value to be written
 * @retVal			-void
 *================================================*/
void MCAL_GPIO_WritePin(GPIOx_typedef* GPIOx , uint16_t pinNumber, uint8_t value);
/*================================================
 * @Fn				-MCAL_GPIO_WritePort
 * @breif			-Read specific port
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-uint16_t Value to be written to the selected port
 * @retVal			-void
 *================================================*/
void MCAL_GPIO_WritePort(GPIOx_typedef* GPIOx, uint16_t value);
/*================================================
 * @Fn				-MCAL_GPIO_ReadPort
 * @breif			-Write to specific port
 * @param			-GPIOx where x can be A, B, or C port
 * @retVal			-uint16_t value that read from the port
 *================================================*/
uint16_t MCAL_GPIO_ReadPort(GPIOx_typedef* GPIOx);

uint8_t MCAL_GPIO_LockPin(GPIOx_typedef* GPIOx, uint16_t pinNumber);
void MCAL_GPIO_TogglePin(GPIOx_typedef* GPIOx , uint16_t pinNumber);

void delay_ms(volatile unsigned int ms);

#endif

