/*
 * gpio.c
 *
 *  Created on: Oct 2, 2025
 *      Author: A&M
 */

#include "gpio.h"

/*================================================
 * @Fn				-MCAL_GPIO_Init
 * @breif			-Initialize the GPIOx PINy according to the specified parameters in the GPIO_Pin_Config
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-pinConfig refers to the pin number and specification
 * @retVal			-none
 *================================================*/

void MCAL_GPIO_Init(GPIOx_typedef* GPIOx , GPIO_Pin_Config* pinConfig)
{
	uint32_t setConfig = 0;

	RCC_GPIOA_CLK_EN();
	RCC_GPIOB_CLK_EN();
	RCC_GPIOC_CLK_EN();

	if((pinConfig->GPIO_PinMode == GPIO_MODE_OUTPUT_OD) || (pinConfig->GPIO_PinMode == GPIO_MODE_OUTPUT_PP) || (pinConfig->GPIO_PinMode == GPIO_MODE_AFIO_PP) || (pinConfig->GPIO_PinMode == GPIO_MODE_AFIO_OD))
	{
		/*OUTPUT MODE VALUE - 4 EQUALS TO THE VALUE MUST BE WRITTEN TO ACTIVATE THAT OUTPUT MODE*/
		setConfig = ((( (pinConfig->GPIO_PinMode - 4) << 2) |(pinConfig->GPIO_PinSpeed)) & 0x0F);
	}
	else
	{
		if((pinConfig->GPIO_PinMode == GPIO_MODE_ANALOG)|| (pinConfig->GPIO_PinMode == GPIO_MODE_INPUT_FLOATING))
		{
			setConfig = ((pinConfig->GPIO_PinMode) << 2) & 0x0F;
		}
		else if(pinConfig->GPIO_PinMode == GPIO_MODE_INPUT_PU) /*Input pull up*/
		{
			setConfig = (0x02 << 2) & 0x0F;
			GPIOx->ODR |= (1 << pinConfig->GPIO_PinNumber); /*Set ODR pin - PxODR = 1*/

		}
		else if(pinConfig->GPIO_PinMode == GPIO_MODE_INPUT_PD)
		{
			setConfig = (0x02 << 2) & 0x0F;
			GPIOx->ODR &= ~(1 << pinConfig->GPIO_PinNumber); /*Reset ODE Pin - PxODR = 0*/

		}
		else if(pinConfig->GPIO_PinMode == GPIO_MODE_AFIO_IN)
		{
			setConfig = (GPIO_MODE_INPUT_FLOATING << 2) & 0x0F; /*Alternative input equals to floating input*/
		}
		else{}
	}
	/* =====================================================================
		for 0 to 7 pins  - > pinPosition in CRL Register = pinNumber + (pinNumber * 3)
		for 8 to 15 pins - > pinPosition in CRH Register = (pinNumber-8) + ((pinNumber-8) * 3)
	=========================================================================*/
	if(pinConfig->GPIO_PinNumber < 8)
	{
		GPIOx->CRL &= ~(0xf <<  (pinConfig->GPIO_PinNumber + (3 * pinConfig->GPIO_PinNumber)));
		GPIOx->CRL |= (setConfig << (pinConfig->GPIO_PinNumber + (3 * pinConfig->GPIO_PinNumber)));
	}
	else
	{
		GPIOx->CRH &= ~(0xF << ((pinConfig->GPIO_PinNumber - 8) + (3 * (pinConfig->GPIO_PinNumber - 8))));
		GPIOx->CRH |= (setConfig << ((pinConfig->GPIO_PinNumber - 8) + (3 * (pinConfig->GPIO_PinNumber - 8))));
	}

}

/*================================================
 * @Fn				-MCAL_GPIO_DeInit
 * @breif			-DeInitialize(Reset) the selected GPIOx port
 * @param			-GPIOx where x can be A, B, or C port
 * @retVal			-none
 *================================================*/
void MCAL_GPIO_DeInit(GPIOx_typedef* GPIOx)
{
	/*Reset using the reset values of all registers.*/
//	GPIOx->BRR 	=  0x00000000;
//	GPIOx->BSRR =  0x00000000;
//	GPIOx->CRL 	=  0x44444444;
//	GPIOx->CRH 	=  0x44444444;
//	GPIOx->ODR 	=  0x00000000;
//	GPIOx->LCKR =  0x00000000;
	/*Reset using the RCC Reset register.*/
	if(GPIOx == GPIOA)
	{
		RCC->APB2RSTR |= (1 << 2);
		RCC->APB2RSTR &= ~(1 << 2);
	}
	else if(GPIOx == GPIOB)
	{
		RCC->APB2RSTR |= (1 << 3);
		RCC->APB2RSTR &= ~(1 << 3);
	}
	else if(GPIOx == GPIOC)
	{
		RCC->APB2RSTR |= (1 << 4);
		RCC->APB2RSTR &= ~(1 << 4);
	}
	else{}
}
/*================================================
 * @Fn				-MCAL_GPIO_ReadPin
 * @breif			-Read specific PIN
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-pinNumber can be one of reference @ref Pin_numbers
 * @retVal			-uint8_t value refers to the state of the pin
 *================================================*/
uint8_t MCAL_GPIO_ReadPin(GPIOx_typedef* GPIOx , uint16_t pinNumber)
{
	uint8_t value = ((GPIOx->IDR >> pinNumber) & 1);
	return value;
}

/*================================================
 * @Fn				-MCAL_GPIO_WritePin
 * @breif			-Read specific PIN
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-pinNumber can be one of reference @ref Pin_numbers
 * @param			-uint8_t value to be written
 * @retVal			-none
 *================================================*/
void MCAL_GPIO_WritePin(GPIOx_typedef* GPIOx , uint16_t pinNumber, uint8_t value)
{
	if(value == FALSE)
	{
		GPIOx->ODR &= ~(1 << pinNumber);
	}
	else
	{
		GPIOx->ODR |= (1 << pinNumber);
	}
}
/*================================================
 * @Fn				-MCAL_GPIO_TogglePin
 * @breif			-Toggle specific PIN
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-pinNumber can be one of reference @ref Pin_numbers
 * @retVal			-none
 *================================================*/
void MCAL_GPIO_TogglePin(GPIOx_typedef* GPIOx , uint16_t pinNumber)
{
	GPIOx->ODR ^= (1 << pinNumber);
}
/*================================================
 * @Fn				-MCAL_GPIO_WritePort
 * @breif			-Read specific port
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-uint16_t Value to be written to the selected port
 * @retVal			-none
 *================================================*/
void MCAL_GPIO_WritePort(GPIOx_typedef* GPIOx, uint16_t value)
{
	GPIOx->ODR = (uint32_t)value;
}
/*================================================
 * @Fn				-MCAL_GPIO_ReadPort
 * @breif			-Write to specific port
 * @param			-GPIOx where x can be A, B, or C port
 * @retVal			-uint16_t value that read from the port
 *================================================*/
uint16_t MCAL_GPIO_ReadPort(GPIOx_typedef* GPIOx)
{
	return (uint16_t)GPIOx->IDR;
}
/*================================================
 * @Fn				-MCAL_GPIO_LockPin
 * @breif			-Lock the configurations on specific pin
 * @param			-GPIOx where x can be A, B, or C port
 * @param			-pinNumber, it can be one of the @ref Pin_numbers
 * @retVal			-none
 *================================================*/
uint8_t MCAL_GPIO_LockPin(GPIOx_typedef* GPIOx, uint16_t pinNumber)
{
	volatile uint32_t tmp = (1 << 16);
	tmp |= (1 << pinNumber);

	//write 1
	GPIOx->LCKR = tmp;
	// write 0
	GPIOx->LCKR = (1 << pinNumber);
	//write 1
	GPIOx->LCKR = tmp;
	// Read 0
	tmp = GPIOx->LCKR;
	if((uint32_t)(GPIOx->LCKR >> 16) & 1)
	{
		return GPIO_RETURN_LOCK_Enabled;
	}
	else
	{
		return GPIO_RETURN_LOCK_ERROR;
	}
}

void delay_ms(volatile unsigned int ms)
{
    for (volatile unsigned int i = 0; i < ms * 800; i++) {
        __asm("nop");
    }
}

