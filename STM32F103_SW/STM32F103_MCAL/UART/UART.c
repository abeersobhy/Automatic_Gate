/*
 * UART.c
 *
 *  Created on: Oct 14, 2025
 *      Author: A&M
 */
#include "UART.h"

/*=======================================================*/
//Array of 3 Pointers to function
void (*USARTx_IRQ_CallBack[3])(void);
/*=======================================================*/
uint32_t PCLK = 0;
/*=======================================================
* Functions definition
* =======================================================*/

void USARTx_Init(USARTx_Config_t* USARTx_Config)
{
	GPIO_Pin_Config TX;
	GPIO_Pin_Config RX;

	if(USARTx_Config->USARTx == USART1)
	{
		RCC_GPIOA_CLK_EN();
		RCC_AFIO_CLK_EN();
		RCC_USART1_CLK_EN();
		PCLK = RCC_GetPCLK2Freq();

		USARTx_IRQ_CallBack[0] = USARTx_Config->USARTx_IRQ_CallBack;

		if(USARTx_Config->USART_Interrupt_Status == USART_RX_withInterrupt_Enable)
		{
			NVIC_USART1_Enable;
			USARTx_Config->USARTx->USART_CR1 |= (1 << 5);
		}
		else
		{
			NVIC_USART1_Disable;
			USARTx_Config->USARTx->USART_CR1 &= ~(1 << 5);
		}

		TX.GPIO_PinNumber = GPIOx_PIN_9;
		TX.GPIO_PinMode = GPIO_MODE_AFIO_PP;
		TX.GPIO_PinSpeed = GPIO_SPEED_10MHZ;

		RX.GPIO_PinNumber = GPIOx_PIN_10;
		RX.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;

		MCAL_GPIO_Init(GPIOA , &TX);
		MCAL_GPIO_Init(GPIOA , &RX);

	}
	else if(USARTx_Config->USARTx == USART2)
	{
		RCC_GPIOA_CLK_EN();
		RCC_AFIO_CLK_EN();
		RCC_USART2_CLK_EN();

		PCLK = RCC_GetPCLK1Freq();

		USARTx_IRQ_CallBack[1] = USARTx_Config->USARTx_IRQ_CallBack;

		if(USARTx_Config->USART_Interrupt_Status == USART_RX_withInterrupt_Enable)
		{
			NVIC_USART2_Enable;
			USARTx_Config->USARTx->USART_CR1 |= (1 << 5);
		}
		else
		{
			NVIC_USART2_Disable;
			USARTx_Config->USARTx->USART_CR1 &= ~(1 << 5);
		}

		TX.GPIO_PinNumber = GPIOx_PIN_2;
		TX.GPIO_PinMode = GPIO_MODE_AFIO_PP;
		TX.GPIO_PinSpeed = GPIO_SPEED_10MHZ;

		RX.GPIO_PinNumber = GPIOx_PIN_3;
		RX.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;

		MCAL_GPIO_Init(GPIOA , &TX);
		MCAL_GPIO_Init(GPIOA , &RX);

	}
	else if(USARTx_Config->USARTx == USART3)
	{
		RCC_GPIOB_CLK_EN();
		RCC_AFIO_CLK_EN();
		RCC_USART3_CLK_EN();
		PCLK = RCC_GetPCLK1Freq();

		USARTx_IRQ_CallBack[2] = USARTx_Config->USARTx_IRQ_CallBack;

		if(USARTx_Config->USART_Interrupt_Status == USART_RX_withInterrupt_Enable)
		{
			NVIC_USART3_Enable;
			USARTx_Config->USARTx->USART_CR1 |= (1 << 5);
		}
		else
		{
			NVIC_USART3_Disable;
			USARTx_Config->USARTx->USART_CR1 &= ~(1 << 5);
		}

		TX.GPIO_PinNumber = GPIOx_PIN_10;
		TX.GPIO_PinMode = GPIO_MODE_AFIO_PP;
		TX.GPIO_PinSpeed = GPIO_SPEED_10MHZ;

		RX.GPIO_PinNumber = GPIOx_PIN_11;
		RX.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;

		MCAL_GPIO_Init(GPIOB , &TX);
		MCAL_GPIO_Init(GPIOB , &RX);

	}
	else{}

	// Program the M bit in USART_CR1 to define the word length
	USARTx_Config->USARTx->USART_CR1 &= ~(1 << 12); //Firstly, reset the M bit
	USARTx_Config->USARTx->USART_CR1 |= (USARTx_Config->USART_DataLength << 12);

	//  Program the number of stop bits in USART_CR2
	USARTx_Config->USARTx->USART_CR2 &= ~(0b11 << 12); //Firstly, reset the 2 stop bits 12, 13
	USARTx_Config->USARTx->USART_CR2 |= (USARTx_Config->USART_StopBits << 12);

	//  Set the TE/ or RE or Both - in USART_CR1 to send/receive data
	if(USARTx_Config->USART_TX_RX_Mode == USART_TX_Only)
	{
		USARTx_Config->USARTx->USART_CR1 |= (1 << 3);  //Transmitter enabled
		USARTx_Config->USARTx->USART_CR1 &= ~(1 << 2); //Receiver disabled
	}
	else if(USARTx_Config->USART_TX_RX_Mode == USART_RX_Only)
	{
		USARTx_Config->USARTx->USART_CR1 &= ~(1 << 3);  //Transmitter Disabled
		USARTx_Config->USARTx->USART_CR1 |=  (1 << 2); //Receiver Enabled
	}
	else if(USARTx_Config->USART_TX_RX_Mode == USART_RX_TX)
	{
		USARTx_Config->USARTx->USART_CR1 |= (1 << 3);  //Transmitter Enabled
		USARTx_Config->USARTx->USART_CR1 |= (1 << 2); //Receiver Enabled
	}
	else{}

	uint32_t USARTDIV = (25 * PCLK) / (4 * USARTx_Config->USART_BaudRate);   // ×25 لتفادي الفاصلة
	uint32_t Mantissa = USARTDIV / 100;
	uint32_t Fraction = (((USARTDIV - (Mantissa * 100)) * 16) + 50) / 100;

	if (Fraction > 15) {
	    Mantissa += 1;
	    Fraction = 0;
	}

	USARTx_Config->USARTx->USART_BRR = (Mantissa << 4) | (Fraction & 0x0F);


	// Enable the USART by writing the UE bit in USART_CR1 register to 1
	USARTx_Config->USARTx->USART_CR1 |= (1 << 13);


}

uint16_t USARTx_Read(USART_typedef* USARTx)
{
	// wait until  The RXNE bit is set. It indicates that the content of the shift register is transferred to the
	// RDR. In other words, data has been received and can be read
	while(((USARTx->USART_SR >> 5)& 0b1) == 0);
	// return - Read the RDR register
	return (uint16_t)USARTx->USART_DR;
}

uint8_t* USARTx_ReceiveString(USART_typedef* USARTx)
{
	static uint8_t arr_char[100];
	uint8_t i = 0;
	while(arr_char[i] != '\n' || i < 100)
	{
		arr_char[i] = USARTx_Read(USARTx);
		i++;
	}
	return arr_char;
}

void  USARTx_Write(USART_typedef* USARTx, uint16_t value)
{
	// wait until TXE=1. This indicates that the transmission of the last frame is complete.
	while(((USARTx->USART_SR >> 7)& 0b1) == 0);
	// Write the value to send in the USART_DR register
	USARTx->USART_DR = (uint16_t)value;
	 while(!((USARTx->USART_SR >> 6) & 1));
}
void  USARTx_SendString(USART_typedef* USARTx, uint8_t* str)
{
	uint8_t i = 0;
	while(str[i] != '\0')
	{
		USARTx_Write(USARTx, (uint16_t)str[i]);
		i++;
	}
}

void USART1_IRQHandler(void)
{
	USARTx_IRQ_CallBack[0]();
	//Clear Pending bit - Clear by writing 1
}

void USART2_IRQHandler(void)
{
	USARTx_IRQ_CallBack[1]();

	//Clear Pending bit - Clear by writing 1
}

void USART3_IRQHandler(void)
{
	USARTx_IRQ_CallBack[2]();

	//Clear Pending bit - Clear by writing 1
}
