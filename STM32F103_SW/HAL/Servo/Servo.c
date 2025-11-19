/*
 * Servo.c
 *
 *  Created on: Nov 18, 2025
 *      Author: Abeer Sobhy
 */
#include "Servo.h"

volatile Timerx_typedef* ServoTimer = TIM2;

void Servo_Attach(uint8_t ServoPin)
{
	if(ServoPin == SERVO_GPIOA_PIN1) //Initialize TIM2
	{
		TIMx_PWM_Init_t PWM_Init =
		{
			.PWMEnable = PWM_Enable,
			.PWM_Interrupt_Enable = PWM_InterruptDisable,
			.CLK_Prescaler = 8,
			.TimerMaxCount = 20000,
			.TIMx = TIM2,
		};
		TIMx_PWM_CH2_Init(&PWM_Init);
		ServoTimer = TIM2;
	}
	else if(ServoPin == SERVO_GPIOA_PIN7) //Initialize TIM3
	{
		TIMx_PWM_Init_t PWM_Init =
		{
			.PWMEnable = PWM_Enable,
			.PWM_Interrupt_Enable = PWM_InterruptDisable,
			.CLK_Prescaler = 8,
			.TimerMaxCount = 20000,
			.TIMx = TIM3,
		};
		TIMx_PWM_CH2_Init(&PWM_Init);
		ServoTimer = TIM3;
	}
	else if(ServoPin == SERVO_GPIOB_PIN7) //Initialize TIM4
	{
		TIMx_PWM_Init_t PWM_Init =
		{
			.PWMEnable = PWM_Enable,
			.PWM_Interrupt_Enable = PWM_InterruptDisable,
			.CLK_Prescaler = 8,
			.TimerMaxCount = 20000,
			.TIMx = TIM4,
		};
		TIMx_PWM_CH2_Init(&PWM_Init);
		ServoTimer = TIM4;
	}
	else
	{
		/*Not Valid*/
	}
}
void Servo_Write(uint8_t angle)
{
	uint16_t duty = 500 + (angle* 11); /*Where 500 & 2500 are minimum and maximum duty for SG90- servo */

	if(ServoTimer == TIM2) //TIM2 Used to Control Servo
	{
		TIMx_CH2_SetDuty(TIM2, duty);
	}
	else if(ServoTimer == TIM3) //TIM3 Used to Control Servo
	{
		TIMx_CH2_SetDuty(TIM3, duty);
	}
	else if(ServoTimer == TIM4) //TIM4 Used to Control Servo
	{
		TIMx_CH2_SetDuty(TIM4, duty);
	}
	else {/*Not Valid*/}
}


