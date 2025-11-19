/*
 * UART.h
 *
 *  Created on: Oct 14, 2025
 *      Author: A&M
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "gpio.h"
#include "RCC.h"

typedef struct
{
	USART_typedef* USARTx;   	// where x can be 1 , 2, 3
	uint8_t    USART_TX_RX_Mode; 	//@ref USART_TX_RX_Mode
	uint8_t    USART_Polling_Status; //@ref USART_Poliing_Status
	uint8_t    USART_Interrupt_Status; //@ref USART_Interrupt_Statu - This parameter will update the USART Interrupt enable bit & the USART NVIC Status
	uint32_t   USART_BaudRate;  // Table 192. for available baud rates
	uint8_t    USART_DataLength;   //@ref USART_DataLength
	uint8_t    USART_StopBits;    //@ref USART_STOP_BIT
	void (*USARTx_IRQ_CallBack)(void);			//Pointer to function that will be execute if EXTI happened

}USARTx_Config_t;

//@ref USART_TX_RX_Mode
#define USART_TX_Only 				0
#define USART_RX_Only 				1
#define USART_RX_TX   				2

//@ref USART_Poliing_Status
#define USART_Poliing_Enable		0
#define USART_Poliing_Disable 		1

//@ref USART_Interrupt_Status
#define USART_RX_withInterrupt_Enable		0
#define USART_RX_withInterrupt_Disable 		1


//@ref USART_DataLength
#define USART_8BIT_DATA 			0
#define USART_9BIT_DATA 			1

//@ref USART_STOP_BIT
#define USART_1_STOP_BIT 			0x00
#define USART_0_5_STOP_BIT 			0x01
#define USART_2_STOP_BIT 			0x02
#define USART_1_5_STOP_BIT 			0x03


void USARTx_Init(USARTx_Config_t* USARTx_Config);

uint16_t USARTx_Read(USART_typedef* USARTx);
void  USARTx_Write(USART_typedef* USARTx, uint16_t value);

uint8_t* USARTx_ReceiveString(USART_typedef* USARTx);
void  USARTx_SendString(USART_typedef* USARTx, uint8_t* str);


#endif /* UART_UART_H_ */
