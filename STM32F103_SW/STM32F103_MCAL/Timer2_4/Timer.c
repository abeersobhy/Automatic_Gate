/*
 *  Timer.c
 *
 *  Created on: Nov 15, 2025
 *  Author: Abeer Sobhy
 */

#include "Timer.h"

volatile uint32_t is_rising = 0;
volatile uint32_t is_falling = 0;
volatile uint8_t is_captured = 0;
volatile uint8_t maxDelay_detected = 0;

volatile Timerx_typedef* DelayTimer;

void TIMx_Init(Timer_Init_t* Timer_Init)
{
	if(Timer_Init->TIMx == TIM2)
	{
		// Enable Clock for Timer
		RCC_TIM2_CLK_EN();
		DelayTimer = TIM2;
	}
	else if(Timer_Init->TIMx == TIM3)
	{
		RCC_TIM3_CLK_EN();
		DelayTimer = TIM3;
	}
	else if(Timer_Init->TIMx == TIM4)
	{
		RCC_TIM4_CLK_EN();
		DelayTimer = TIM4;
	}
	else{/*Not Valid.*/}
	 // --- Setup Timer ---
	Timer_Init->TIMx->TIMx_ARR = 0xFFFF;
	Timer_Init->TIMx->TIMx_PSC = Timer_Init->CLK_Prescaler - 1;   // 1MHz timer clock => 1us per tick
	Timer_Init->TIMx->TIMx_CR1 |= (1 << 0); 					 // CEN = 1 -> start counter
}
void TIMx_IC_CH1_Init(TIMx_IC_Init_t* IC_Init)
{
	// Set the CH1 Pin as Input Floating
	GPIO_Pin_Config PIN;
	if(IC_Init->InputCaptureEnable == InputCapture_Enable)
	{
		if(IC_Init->TIMx == TIM2)
		{
			// Enable Clock for Timer
			RCC_GPIOA_CLK_EN();
			RCC_TIM2_CLK_EN();
			if(IC_Init->IC_Interrupt_Enable == IC_InterruptEnable)
			{
				IC_Init->TIMx->TIMx_DIER |= (1 << 1);
				NVIC_TIM2_GI_Enable;
			}
			else
			{
				IC_Init->TIMx->TIMx_DIER &= ~(1 << 1);
				NVIC_TIM2_GI_Disable;
			}

			PIN.GPIO_PinNumber = GPIOx_PIN_0;
			PIN.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;
			MCAL_GPIO_Init(GPIOA , &PIN);
		}
		else if(IC_Init->TIMx == TIM3)
		{
			RCC_GPIOA_CLK_EN();
			RCC_GPIOB_CLK_EN();
			RCC_TIM3_CLK_EN();
			if(IC_Init->IC_Interrupt_Enable == IC_InterruptEnable)
			{
				IC_Init->TIMx->TIMx_DIER |= (1 << 1);
				NVIC_TIM3_GI_Enable;
			}
			else
			{
				IC_Init->TIMx->TIMx_DIER &= ~(1 << 1);
				NVIC_TIM3_GI_Disable;
			}

			PIN.GPIO_PinNumber = GPIOx_PIN_6;
			PIN.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;
			MCAL_GPIO_Init(GPIOA , &PIN);
		}
		else if(IC_Init->TIMx == TIM4)
		{
			RCC_GPIOB_CLK_EN();
			RCC_TIM4_CLK_EN();
			if(IC_Init->IC_Interrupt_Enable == IC_InterruptEnable)
			{
				IC_Init->TIMx->TIMx_DIER |= (1 << 1);
				NVIC_TIM4_GI_Enable;
			}
			else
			{
				IC_Init->TIMx->TIMx_DIER &= ~(1 << 1);
				NVIC_TIM4_GI_Disable;
			}
			PIN.GPIO_PinNumber = GPIOx_PIN_6;
			PIN.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;
			MCAL_GPIO_Init(GPIOB , &PIN);
		}
		else{/*Not Valid.*/}

		// Setup GPIO
		/* Select the active input: TIMx_CCR1 must be linked to the TI1 input, so write the CC1S
		bits to 01 in the TIMx_CCMR1 register. As soon as CC1S becomes different from 00,
		the channel is configured in input and the TIMx_CCR1 register becomes read-only.
		 * */
		IC_Init->TIMx->TIMx_CCMR1 &= ~(0xFF);      // Clear CCMR1
		IC_Init->TIMx->TIMx_CCMR1 |= (1 << 0);     // Set 01 to CC1S bits
		/*
		 *  Program the needed input filter duration with respect to the signal connected to the
			timer (by programming the ICxF bits in the TIMx_CCMRx register if the input is one of
			the TIx inputs).
		*/
		// 0011: fSAMPLING=fCK_INT, N=8
		IC_Init->TIMx->TIMx_CCMR1 |= (3 << 4);
		IC_Init->TIMx->TIMx_CCMR1 &= ~(0xF << 4); // Clear filter bits
		/*
		 Program the input prescaler. In our example, we wish the capture to be performed at
		 each valid transition, so the prescaler is disabled (write IC1PS bits to 00 in the
		 TIMx_CCMR1 register)
		 */
		IC_Init->TIMx->TIMx_CCMR1 &= ~(0b11 << 2);
		/*
			Select the edge of the active transition on the TI1 channel by writing the CC1P bit to 0
			in the TIMx_CCER register (rising edge in this case)
		*/
		IC_Init->TIMx->TIMx_CCER &= ~(1 << 1); // rising edge
		/*
		   Enable capture from the counter into the capture register by setting the CC1E bit in the
		   TIMx_CCER register
		*/
		IC_Init->TIMx->TIMx_CCER |= (1 << 0); // CC1E = 1 -> enable capture

		/*
		Enable the related interrupt request by setting the CC1IE bit in the
		TIMx_DIER register
		*/
		IC_Init->TIMx->TIMx_PSC = IC_Init->CLK_Prescaler - 1;   // 1MHz resolution (1us per tick)
		IC_Init->TIMx->TIMx_ARR = 0xFFFF;  // Max count (just to be safe)

		IC_Init->TIMx->TIMx_CR1 |= (1 << 0);  // CEN = 1 -> start counter
	}
}
void TIMx_PWM_CH2_Init(TIMx_PWM_Init_t* PWM_Init)
{
	// Set the CH1 Pin as AF Push pull
	GPIO_Pin_Config PIN;
	if(PWM_Init->TIMx == TIM2)
	{
		RCC_GPIOA_CLK_EN();
		RCC_TIM2_CLK_EN();
		PIN.GPIO_PinNumber = GPIOx_PIN_1;
		PIN.GPIO_PinMode = GPIO_MODE_AFIO_PP;
		PIN.GPIO_PinSpeed = GPIO_SPEED_50MHZ;
		MCAL_GPIO_Init(GPIOA , &PIN);
	}
	else if(PWM_Init->TIMx == TIM3)
	{
		RCC_GPIOA_CLK_EN();
		RCC_GPIOB_CLK_EN();
		RCC_TIM3_CLK_EN();
		PIN.GPIO_PinNumber = GPIOx_PIN_7;
		PIN.GPIO_PinMode = GPIO_MODE_AFIO_PP;
		PIN.GPIO_PinSpeed = GPIO_SPEED_50MHZ;
		MCAL_GPIO_Init(GPIOA , &PIN);
	}
	else if(PWM_Init->TIMx == TIM4)
	{
		RCC_GPIOB_CLK_EN();
		RCC_TIM4_CLK_EN();
		PIN.GPIO_PinNumber = GPIOx_PIN_7;
		PIN.GPIO_PinMode = GPIO_MODE_AFIO_PP;
		PIN.GPIO_PinSpeed = GPIO_SPEED_50MHZ;
		MCAL_GPIO_Init(GPIOB , &PIN);
	}
	else{/*Not Valid.*/}

	PWM_Init->TIMx->TIMx_PSC = PWM_Init->CLK_Prescaler - 1;        // Timer clock = 1 MHz (assuming 8MHz system clock)
	PWM_Init->TIMx->TIMx_ARR = PWM_Init->TimerMaxCount - 1;     // 20ms period for Servo (50Hz)
	PWM_Init->TIMx->TIMx_CNT = 0;             					// Reset counter

	PWM_Init->TIMx->TIMx_CCMR1 &= ~(0xFF << 8);   // Clear CC2S + OC2M + OC2PE bits

	PWM_Init->TIMx->TIMx_CCMR1 |= (6 << 12);      // OC2M = 110 → PWM mode 1

	PWM_Init->TIMx->TIMx_CCMR1 |= (1 << 11);      // OC2PE = 1 → preload enable
	PWM_Init->TIMx->TIMx_CCMR1 &= ~(0x3 << 8);    // CC2S = 00 → output

	PWM_Init->TIMx->TIMx_CCER |= (1 << 4);       // CC2E = 1 → enable CH2 output

	PWM_Init->TIMx->TIMx_CCER &= ~(1 << 5);      // CC2P = 0 → active high

	PWM_Init->TIMx->TIMx_CR1 |= (1 << 7);       // ARPE = 1 → preload enable

	PWM_Init->TIMx->TIMx_CR1 |= (1 << 0);       // CEN = 1 → start counter

}
void TIMx_CH2_SetDuty(Timerx_typedef* TIMx, uint16_t pulse_us)
{
	TIMx->TIMx_CCR2 = pulse_us;   // pulse_us in microseconds
}
uint32_t TIMx_GetHighWidth(Timerx_typedef* TIMx)
{
	if(is_captured)
	{
	    uint32_t width;
	    if(is_falling >= is_rising)
	        width = is_falling - is_rising;
	    else
	        width = (TIMx->TIMx_ARR - is_rising) + is_falling + 1;

	    is_captured = 0; // Reset flag
	    // width now in timer ticks → convert to µs
	    return width; // if PSC = 1 MHz → 1 tick = 1 µs
	}
	return 0;
}
void TIMx_delay_us(uint32_t delayInUS)
{
	if(DelayTimer == TIM2)
	{
		TIM2->TIMx_CNT = 0;                      // Reset counter
		while(TIM2->TIMx_CNT < delayInUS);       // Wait until CNT reaches desired value
	}
	else if(DelayTimer == TIM3)
	{
		TIM3->TIMx_CNT = 0;                      // Reset counter
		while(TIM3->TIMx_CNT < delayInUS);       // Wait until CNT reaches desired value
	}
	else if(DelayTimer == TIM4)
	{
		TIM4->TIMx_CNT = 0;                      // Reset counter
		while(TIM4->TIMx_CNT < delayInUS);       // Wait until CNT reaches desired value
	}
	else{/*Not Valid.*/}
}
void TIMx_delay_ms(uint32_t delayInMS)
{
    for(uint16_t i = 0; i < delayInMS; i++)
    {
    	TIMx_delay_us(1000);           // 1 ms = 1000 µs
    }
}
void TIM2_IRQHandler(void)
{
	if((TIM2->TIMx_SR >> 1) & 0x1)  // Check CC1 interrupt flag, Capture occurred
	{
		static uint8_t rising = 1;
	    if((rising == 1) && (is_captured == 0))
	    {
	    	is_rising = TIM2->TIMx_CCR1;    // Capture rising edge
	        rising = 0;
	        TIM2->TIMx_CCER |= (1 << 1); // Switch to falling edge
	    }
	    else if((rising == 0) && (is_captured == 0))
	    {
	        is_falling = TIM2->TIMx_CCR1;   // Capture falling edge
	        rising = 1;
	        is_captured = 1;           // Pulse measurement ready
	        TIM2->TIMx_CCER &= ~(1 << 1); // Switch back to rising edge
	    }
	    TIM2->TIMx_SR &= ~(1 << 1);      // Clear interrupt flag
	}
}
void TIM3_IRQHandler(void)
{
	if((TIM3->TIMx_SR >> 1) & 0x1)     // Check CC1 interrupt flag, Capture occurred
	{
		static uint8_t rising = 1;
		if((rising == 1) && (is_captured == 0))
	    {
	    	is_rising = TIM3->TIMx_CCR1;    // Capture rising edge
	        rising = 0;
	        TIM3->TIMx_CCER |= (1 << 1); // Switch to falling edge
	    }
		else if((rising == 0) && (is_captured == 0))
	    {
	        is_falling = TIM3->TIMx_CCR1;   // Capture falling edge
	        rising = 1;
	        is_captured = 1;           // Pulse measurement ready
	        TIM3->TIMx_CCER &= ~(1 << 1); // Switch back to rising edge
	    }
	    TIM3->TIMx_SR &= ~(1 << 1);      // Clear interrupt flag
	}
}
void TIM4_IRQHandler(void)
{
	if((TIM4->TIMx_SR >> 1) & 0x1)  // Check CC1 interrupt flag, Capture occurred
	{
		static uint8_t rising = 1;
		if((rising == 1) && (is_captured == 0))
	    {
	    	is_rising = TIM4->TIMx_CCR1;    // Capture rising edge
	        rising = 0;
	        TIM4->TIMx_CCER |= (1 << 1); // Switch to falling edge
	    }
	    else if((rising == 0) && (is_captured == 0))
	    {
	        is_falling = TIM4->TIMx_CCR1;   // Capture falling edge
	        rising = 1;
	        is_captured = 1;           // Pulse measurement ready
	        TIM4->TIMx_CCER &= ~(1 << 1); // Switch back to rising edge
	    }
	    TIM4->TIMx_SR &= ~(1 << 1);      // Clear interrupt flag
	}
}
