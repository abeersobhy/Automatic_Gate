/*
 *  Timer.h
 *  Created on: Nov 15, 2025
 *  Author: Abeer Sobhy
 */
#ifndef TIMER2_4_TIMER_H_
#define TIMER2_4_TIMER_H_

#include "gpio.h"

typedef struct
{
	uint32_t CLK_Prescaler;
	uint32_t Timer_Init_Value;
	uint32_t Timer_GI_Enable;   // Enable or disable
	uint32_t UpcountingMode_Enable; // Enable or disable
	uint32_t DowncountingMode_Enable; // Enable or disable
	Timerx_typedef* TIMx;       // TIM2 OR TIM3 OR TIM4
}Timer_Init_t;

typedef struct
{
	Timerx_typedef* TIMx;       // TIM2 OR TIM3 OR TIM4
	uint32_t InputCaptureEnable;
	uint32_t IC_Interrupt_Enable;
	uint32_t CLK_Prescaler;
}TIMx_IC_Init_t;

typedef struct
{
	Timerx_typedef* TIMx;       // TIM2 OR TIM3 OR TIM4
	uint32_t PWMEnable;
	uint32_t PWM_Mode;		    // Fast mode or Phase Correct
	uint32_t PWM_Interrupt_Enable;
	uint32_t TimerMaxCount;
	uint32_t CLK_Prescaler;
}TIMx_PWM_Init_t;

#define CH1							1
#define CH2							2
#define CH3							3
#define CH4							4

#define 	PWM_Enable				1
#define 	PWM_Disable				0

#define 	PWM_InterruptEnable		1
#define 	PWM_InterruptDisable	0

#define 	InputCapture_Enable		1
#define 	InputCapture_Disable	0

#define 	IC_InterruptEnable		1
#define 	IC_InterruptDisable		0

#define 	TIM_GI_Enable			1
#define 	TIM_GI_Disable			0

#define 	Count_UP_Enable			0
#define 	Count_DOWN_Enable		1

void TIMx_Init(Timer_Init_t* Timer_Init);

void TIMx_IC_CH1_Init(TIMx_IC_Init_t* IC_Init);
uint32_t TIMx_GetHighWidth(Timerx_typedef* TIMx);
void TIMx_PWM_CH2_Init(TIMx_PWM_Init_t* PWM_Init);
void TIMx_CH2_SetDuty(Timerx_typedef* TIMx, uint16_t pulse_us);
void TIMx_delay_us(uint32_t delayInUS);
void TIMx_delay_ms(uint32_t delayInMS);

#endif /* TIMER2_4_TIMER_H_ */
