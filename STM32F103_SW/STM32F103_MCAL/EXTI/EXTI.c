/*
 * EXTI.c
 *
 *  Created on: Oct 8, 2025
 *  Author: A&M
 */


#include "EXTI.h"
/*=======================================================*/
//Array of 15 Pointers to function
void (*GP_IRQ_CallBack[15])(void);
/*=======================================================*/
// Function prototypes
void NVIC_Enable	(uint8_t);
void NVIC_Disable	(uint8_t);
/*=======================================================
* Functions definition
* =======================================================*/
void EXTI_GPIO_Init(EXTI_PinConfig_t* EXTI_Config)
{
	/*Enable CLK of AFIO.*/
	RCC_AFIO_CLK_EN();

	// 1. Configure the pin as AFIO Input
	GPIO_Pin_Config pin =
	{.GPIO_PinNumber = EXTI_Config->EXTI_PIN_Mapping.pinNumber,
	 .GPIO_PinMode = GPIO_MODE_AFIO_IN,
	};
	MCAL_GPIO_Init(EXTI_Config->EXTI_PIN_Mapping.GPIOx, &pin);
	/*====================================================================*/
	// 1. Select between PORTA, B, and C Pins
	uint16_t selectedPort;
	if(EXTI_Config->EXTI_PIN_Mapping.GPIOx == GPIOA)
	{
		selectedPort = 0x0000;
	}
	else if(EXTI_Config->EXTI_PIN_Mapping.GPIOx == GPIOB)
	{
		selectedPort = 0x0001;
	}
	else if(EXTI_Config->EXTI_PIN_Mapping.GPIOx == GPIOC)
	{
		selectedPort = 0x0010;
	}
	else{}
	/*==================================================================*/
	// 2.1. Assign the registers
	if(EXTI_Config->EXTI_PIN_Mapping.EXTIx < 4)
	{
		AFIO->EXTICR1 &= ~(0xF << (EXTI_Config->EXTI_PIN_Mapping.EXTIx*4)); //Make sure that the bits are clear
		AFIO->EXTICR1 |= (selectedPort << (EXTI_Config->EXTI_PIN_Mapping.EXTIx*4));
	}
	else if(EXTI_Config->EXTI_PIN_Mapping.EXTIx < 8)
	{
		AFIO->EXTICR2 &= ~(0xF << ((EXTI_Config->EXTI_PIN_Mapping.EXTIx - 4)* 4)); //Make sure that the bits are clear
		AFIO->EXTICR2 |= (selectedPort << ((EXTI_Config->EXTI_PIN_Mapping.EXTIx - 4)* 4));
	}
	else if(EXTI_Config->EXTI_PIN_Mapping.EXTIx < 12)
	{
		AFIO->EXTICR3 &= ~(0xF << ((EXTI_Config->EXTI_PIN_Mapping.EXTIx - 8)* 4)); //Make sure that the bits are clear
		AFIO->EXTICR3 |= (selectedPort << ((EXTI_Config->EXTI_PIN_Mapping.EXTIx - 8)* 4));
	}
	else if(EXTI_Config->EXTI_PIN_Mapping.EXTIx < 16)
	{
		AFIO->EXTICR4 &= ~(0xF << ((EXTI_Config->EXTI_PIN_Mapping.EXTIx - 12)* 4)); //Make sure that the bits are clear
		AFIO->EXTICR4 |= (selectedPort << ((EXTI_Config->EXTI_PIN_Mapping.EXTIx - 12)* 4));
	}
	else{}
	/*====================================================================*/
	// 3. Interrupt Trigger Falling or Rising
	if(EXTI_Config->triggerCase == EXTIx_Trigger_RISING)
	{
		EXTI->RTSR &= ~(1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx);
		EXTI->RTSR |= (1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx);
	}
	else if(EXTI_Config->triggerCase == EXTIx_Trigger_FALLING)
	{
		EXTI->FTSR &= ~(1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx);
		EXTI->FTSR |= (1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx);
	}
	else
	{
		EXTI->RTSR &= ~(1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx);
		EXTI->RTSR |= (1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx);

		EXTI->FTSR &= ~(1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx);
		EXTI->FTSR |= (1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx);
	}
	/*====================================================================*/
	// 4. Enable Interrupt
	if(EXTI_Config->EXTIx_Enable == EXTIx_ENABLE)
	{
		EXTI->IMR |= (1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx); //EXTI Enable
		NVIC_Enable(EXTI_Config->EXTI_PIN_Mapping.EXTIx); //NVIC Enable
	}
	else
	{
		EXTI->IMR &= ~(1 << EXTI_Config->EXTI_PIN_Mapping.EXTIx); // EXTI Disable
		NVIC_Disable(EXTI_Config->EXTI_PIN_Mapping.EXTIx); //NVIC Disable
	}
	// 5. Update the Handling call back function
	GP_IRQ_CallBack[EXTI_Config->EXTI_PIN_Mapping.EXTIx] = EXTI_Config->P_IRQ_CallBack;
}
void NVIC_Enable(uint8_t IR_Line)
{
	switch(IR_Line)
		{
		case EXTI0:
			NVIC_EXTI0_Enable;
			break;
		case EXTI1:
				NVIC_EXTI1_Enable;
				break;
		case EXTI2:
				NVIC_EXTI2_Enable;
				break;
		case EXTI3:
				NVIC_EXTI3_Enable;
				break;
		case EXTI4:
				NVIC_EXTI4_Enable;
				break;
		case EXTI5:
		case EXTI6:
		case EXTI7:
		case EXTI8:
		case EXTI9:
			NVIC_EXTI5_9_Enable;
				break;
		case EXTI10:
		case EXTI11:
		case EXTI12:
		case EXTI13:
		case EXTI14:
		case EXTI15:
			NVIC_EXTI10_15_Enable;
				break;
		}
}
void NVIC_Disable(uint8_t IR_Line)
{
	switch(IR_Line)
		{
		case EXTI0:
			NVIC_EXTI0_Disable;
			break;
		case EXTI1:
			NVIC_EXTI1_Disable;
			break;
		case EXTI2:
			NVIC_EXTI2_Disable;
			break;
		case EXTI3:
			NVIC_EXTI3_Disable;
			break;
		case EXTI4:
			NVIC_EXTI4_Disable;
			break;
		case EXTI5:
		case EXTI6:
		case EXTI7:
		case EXTI8:
		case EXTI9:
			NVIC_EXTI5_9_Disable;
				break;
		case EXTI10:
		case EXTI11:
		case EXTI12:
		case EXTI13:
		case EXTI14:
		case EXTI15:
			NVIC_EXTI10_15_Disable;
				break;
		}
}
void EXTI_GPIO_DeInit(void)
{
	EXTI->IMR   = 0x00000000;
	EXTI->EMR   = 0x00000000;
	EXTI->RTSR  = 0x00000000;
	EXTI->FTSR  = 0x00000000;
	EXTI->SWIER = 0x00000000;
	EXTI->PR	= 0xFFFFFFFF; //cleared by writing a '1'
	NVIC_EXTI0_Disable;
	NVIC_EXTI1_Disable;
	NVIC_EXTI2_Disable;
	NVIC_EXTI3_Disable;
	NVIC_EXTI4_Disable;
	NVIC_EXTI5_9_Disable;
	NVIC_EXTI10_15_Disable;
}
void EXTI0_IRQHandler(void)
{
	GP_IRQ_CallBack[0]();
	//Clear Pending bit - Clear by writing 1
	EXTI->PR |= (1 << 0);
}
void EXTI1_IRQHandler(void)
{
	GP_IRQ_CallBack[1]();
	//Clear Pending bit - Clear by writing 1
	EXTI->PR |= (1 << 1);

}
void EXTI2_IRQHandler(void)
{
	GP_IRQ_CallBack[2]();
	//Clear Pending bit - Clear by writing 1
	EXTI->PR |= (1 << 2);

}
void EXTI3_IRQHandler(void)
{
	GP_IRQ_CallBack[3]();
	//Clear Pending bit - Clear by writing 1
	EXTI->PR |= (1 << 3);

}
void EXTI4_IRQHandler(void)
{
	GP_IRQ_CallBack[4]();
	//Clear Pending bit - Clear by writing 1
	EXTI->PR |= (1 << 4);
}
void EXTI9_5_IRQHandler(void)
{
	if(GET_BIT(EXTI->PR , 5))
	{
		GP_IRQ_CallBack[5]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 5);
	}
	if(GET_BIT(EXTI->PR , 6))
	{
		GP_IRQ_CallBack[6]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 6);
	}
	if(GET_BIT(EXTI->PR , 7))
	{
		GP_IRQ_CallBack[7]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 7);
	}
	if(GET_BIT(EXTI->PR , 8))
	{
		GP_IRQ_CallBack[8]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 8);
	}
	if(GET_BIT(EXTI->PR , 9))
	{
		GP_IRQ_CallBack[9]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 9);
	}
}
void EXTI15_10_IRQHandler(void)
{
	if(GET_BIT(EXTI->PR , 10))
	{
		GP_IRQ_CallBack[10]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 10);
	}
	if(GET_BIT(EXTI->PR , 11))
	{
		GP_IRQ_CallBack[11]();
	//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 11);
	}
	if(GET_BIT(EXTI->PR , 12))
	{
		GP_IRQ_CallBack[12]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 12);
	}
	if(GET_BIT(EXTI->PR , 13))
	{
		GP_IRQ_CallBack[13]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 13);
	}
	if(GET_BIT(EXTI->PR , 14))
	{
		GP_IRQ_CallBack[14]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 14);
	}
	if(GET_BIT(EXTI->PR , 15))
	{
		GP_IRQ_CallBack[15]();
		//Clear Pending bit - Clear by writing 1
		EXTI->PR |= (1 << 15);
	}
}
