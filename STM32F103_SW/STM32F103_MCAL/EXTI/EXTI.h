/*
 * EXTI.h
 *
 *  Created on: Oct 8, 2025
 *      Author: A&M
 */

#ifndef EXTI_EXTI_H_
#define EXTI_EXTI_H_

#include "stm32f103xx.h"
#include "gpio.h"

// EXTIx where x any be any number form 0 to 15
// @ref EXTI_LineNumber

#define EXTI0			0
#define EXTI1			1
#define EXTI2			2
#define EXTI3			3
#define EXTI4			4
#define EXTI5			5
#define EXTI6			6
#define EXTI7			7
#define EXTI8			8
#define EXTI9			9
#define EXTI10			10
#define EXTI11			11
#define EXTI12			12
#define EXTI13			13
#define EXTI14			14
#define EXTI15			15
/*==========================================================================================*/
//@ref TriggerCaseMACRO
#define EXTIx_Trigger_RISING				0
#define EXTIx_Trigger_FALLING				1
#define EXTIx_Trigger_RisingAndFalling		2

/*=========================================================================================*/
// @ref EXTIx_STATUS
#define EXTIx_DISABLE						0
#define EXTIx_ENABLE						1

/*=========================================================================================*/
// @ref EXTI_define

#define EXTI0PA0			(EXTI_GPIO_Maping_t){EXTI0, GPIOx_PIN_0, GPIOA, EXTI0_IRQ}
#define EXTI0PB0			(EXTI_GPIO_Maping_t){EXTI0, GPIOx_PIN_0, GPIOB, EXTI0_IRQ}


#define EXTI1PA1			(EXTI_GPIO_Maping_t){EXTI1, GPIOx_PIN_1, GPIOA, EXTI1_IRQ}
#define EXTI1PB1			(EXTI_GPIO_Maping_t){EXTI1, GPIOx_PIN_1, GPIOB, EXTI1_IRQ}

#define EXTI2PA2			(EXTI_GPIO_Maping_t){EXTI2, GPIOx_PIN_2, GPIOA, EXTI2_IRQ}
#define EXTI2PB2			(EXTI_GPIO_Maping_t){EXTI2, GPIOx_PIN_2, GPIOB, EXTI2_IRQ}

#define EXTI3PA3			(EXTI_GPIO_Maping_t){EXTI3, GPIOx_PIN_3, GPIOA, EXTI3_IRQ}
#define EXTI3PB3			(EXTI_GPIO_Maping_t){EXTI3, GPIOx_PIN_3, GPIOB, EXTI3_IRQ}

#define EXTI4PA4			(EXTI_GPIO_Maping_t){EXTI4, GPIOx_PIN_4, GPIOA, EXTI4_IRQ}
#define EXTI4PB4			(EXTI_GPIO_Maping_t){EXTI4, GPIOx_PIN_4, GPIOB, EXTI4_IRQ}

#define EXTI5PA5			(EXTI_GPIO_Maping_t){EXTI5, GPIOx_PIN_5, GPIOA, EXTI5_IRQ}
#define EXTI5PB5			(EXTI_GPIO_Maping_t){EXTI5, GPIOx_PIN_5, GPIOB, EXTI5_IRQ}

#define EXTI6PA6			(EXTI_GPIO_Maping_t){EXTI6, GPIOx_PIN_6, GPIOA, EXTI6_IRQ}
#define EXTI6PB6			(EXTI_GPIO_Maping_t){EXTI6, GPIOx_PIN_6, GPIOB, EXTI6_IRQ}

#define EXTI7PA7			(EXTI_GPIO_Maping_t){EXTI7, GPIOx_PIN_7, GPIOA, EXTI7_IRQ}
#define EXTI7PB7			(EXTI_GPIO_Maping_t){EXTI7, GPIOx_PIN_7, GPIOB, EXTI7_IRQ}

#define EXTI8PA8			(EXTI_GPIO_Maping_t){EXTI8, GPIOx_PIN_8, GPIOA, EXTI8_IRQ}
#define EXTI8PB8			(EXTI_GPIO_Maping_t){EXTI8, GPIOx_PIN_8, GPIOB, EXTI8_IRQ}

#define EXTI9PA9			(EXTI_GPIO_Maping_t){EXTI9, GPIOx_PIN_9, GPIOA, EXTI9_IRQ}
#define EXTI9PB9			(EXTI_GPIO_Maping_t){EXTI9, GPIOx_PIN_9, GPIOB, EXTI9_IRQ}

#define EXTI10PA10			(EXTI_GPIO_Maping_t){EXTI10, GPIOx_PIN_10, GPIOA, EXTI10_IRQ}
#define EXTI10PB10			(EXTI_GPIO_Maping_t){EXTI10, GPIOx_PIN_10, GPIOB, EXTI10_IRQ}

#define EXTI11PA11			(EXTI_GPIO_Maping_t){EXTI11, GPIOx_PIN_11, GPIOA, EXTI11_IRQ}
#define EXTI11PB11			(EXTI_GPIO_Maping_t){EXTI11, GPIOx_PIN_11, GPIOB, EXTI11_IRQ}

#define EXTI12PA12			(EXTI_GPIO_Maping_t){EXTI12, GPIOx_PIN_12, GPIOA, EXTI12_IRQ}
#define EXTI12PB12			(EXTI_GPIO_Maping_t){EXTI12, GPIOx_PIN_12, GPIOB, EXTI12_IRQ}

#define EXTI13PA13			(EXTI_GPIO_Maping_t){EXTI13, GPIOx_PIN_13, GPIOA, EXTI13_IRQ}
#define EXTI13PB13			(EXTI_GPIO_Maping_t){EXTI13, GPIOx_PIN_13, GPIOB, EXTI13_IRQ}
#define EXTI13PC13			(EXTI_GPIO_Maping_t){EXTI13, GPIOx_PIN_13, GPIOC, EXTI13_IRQ}

#define EXTI14PA14			(EXTI_GPIO_Maping_t){EXTI14, GPIOx_PIN_14, GPIOA, EXTI14_IRQ}
#define EXTI14PB14			(EXTI_GPIO_Maping_t){EXTI14, GPIOx_PIN_14, GPIOB, EXTI14_IRQ}
#define EXTI14PC14			(EXTI_GPIO_Maping_t){EXTI14, GPIOx_PIN_14, GPIOC, EXTI14_IRQ}

#define EXTI15PA15			(EXTI_GPIO_Maping_t){EXTI15, GPIOx_PIN_15, GPIOA, EXTI15_IRQ}
#define EXTI15PB15			(EXTI_GPIO_Maping_t){EXTI15, GPIOx_PIN_15, GPIOB, EXTI15_IRQ}
#define EXTI15PC15			(EXTI_GPIO_Maping_t){EXTI15, GPIOx_PIN_15, GPIOC, EXTI15_IRQ}


/*Port Selection*/

#define SELECT_PORTA		0x00
#define SELECT_PORTB		0x01
#define SELECT_PORTC		0x02


typedef struct
{
	uint8_t 		EXTIx; 		// where x can be any number from 0 to 15
	uint8_t 		pinNumber;	// refers to @ref Pin_numbers
	GPIOx_typedef* 	GPIOx;		// x can be A,B,or C
	uint8_t 		EXTIx_IRQ;  // x can be any number from 0 to 15 @ref EXTIx_IRQ
}EXTI_GPIO_Maping_t;


typedef struct
{
	EXTI_GPIO_Maping_t EXTI_PIN_Mapping;	//this parameter must be set based on @ref EXTI_define
	uint8_t triggerCase;					//this parameter must be set based on @ref TriggerCaseMACRO
	uint8_t EXTIx_Enable;					//this parameter must be set based on @ref EXTIx_STATUS
	void (*P_IRQ_CallBack)(void);			//Pointer to function that will be execute if EXTI happened
}EXTI_PinConfig_t;	 //this parameter will reflect on the IRQ Mask register and also the NVIC Interrupt enable register




void EXTI_GPIO_Init(EXTI_PinConfig_t* EXTI_Config);
void EXTI_GPIO_DeInit(void);
#endif /* EXTI_EXTI_H_ */
