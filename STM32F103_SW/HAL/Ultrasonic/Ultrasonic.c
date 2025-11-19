/*
 * Ultrasonic.c
 *
 *  Created on: Nov 17, 2025
 *  Author: Abeer Sobhy
 */
#include "LCD.h"
#include "Ultrasonic.h"


void Ultrasonic_Init(Ultrasonic_Init_t* ultrasonic_init)
{
	Timer_Init_t Timer_Init =
	{
		.CLK_Prescaler = 8,
		.TIMx = TIM4,
	};
	TIMx_Init(&Timer_Init);

	if(ultrasonic_init->echo_pin == ECHO_GPIOA_PIN0)
	{
		TIMx_IC_Init_t IC_Init =
		{
			.IC_Interrupt_Enable = IC_InterruptEnable,
			.InputCaptureEnable = InputCapture_Enable,
			.CLK_Prescaler = 8,
			.TIMx = TIM2,
		};
		TIMx_IC_CH1_Init(&IC_Init);
	}
	else if(ultrasonic_init->echo_pin == ECHO_GPIOA_PIN6)
	{
		TIMx_IC_Init_t IC_Init =
		{
			.IC_Interrupt_Enable = IC_InterruptEnable,
			.InputCaptureEnable = InputCapture_Enable,
			.CLK_Prescaler = 8,
			.TIMx = TIM3,
		};
		TIMx_IC_CH1_Init(&IC_Init);
	}
	else if(ultrasonic_init->echo_pin == ECHO_GPIOB_PIN6)
	{
		TIMx_IC_Init_t IC_Init =
		{
			.IC_Interrupt_Enable = IC_InterruptEnable,
			.InputCaptureEnable = InputCapture_Enable,
			.CLK_Prescaler = 8,
			.TIMx = TIM4,
		};
		TIMx_IC_CH1_Init(&IC_Init);
	}
	else{/*Not Valid.*/}
	GPIO_Pin_Config outPin =
	{
		.GPIO_PinMode = GPIO_MODE_OUTPUT_PP,
		.GPIO_PinNumber = ultrasonic_init->trig_pin,
		.GPIO_PinSpeed = GPIO_SPEED_10MHZ,
	};
	MCAL_GPIO_Init(ultrasonic_init->trig_port, &outPin);
}
double Ultrasonic_GetDistance(Ultrasonic_Init_t* ultrasonic_init)
{
	uint16_t timeInMicroSeconds = 0;
	double distance = 0;

	MCAL_GPIO_WritePin(ultrasonic_init->trig_port, ultrasonic_init->trig_pin, FALSE);
	TIMx_delay_us(5);

	MCAL_GPIO_WritePin(ultrasonic_init->trig_port, ultrasonic_init->trig_pin, TRUE);
	TIMx_delay_us(10);

	MCAL_GPIO_WritePin(ultrasonic_init->trig_port, ultrasonic_init->trig_pin, FALSE);
	TIMx_delay_us(10);

	if(ultrasonic_init->echo_pin == ECHO_GPIOA_PIN0)
	{
		timeInMicroSeconds = TIMx_GetHighWidth(TIM2);
	}
	else if(ultrasonic_init->echo_pin == ECHO_GPIOA_PIN6)
	{
		timeInMicroSeconds = TIMx_GetHighWidth(TIM3);
	}
	else if(ultrasonic_init->echo_pin == ECHO_GPIOB_PIN6)
	{
		timeInMicroSeconds = TIMx_GetHighWidth(TIM4);
	}
	else{/*Not Valid.*/}
	distance = (timeInMicroSeconds * 0.0343) / 2;

	return distance;
}
