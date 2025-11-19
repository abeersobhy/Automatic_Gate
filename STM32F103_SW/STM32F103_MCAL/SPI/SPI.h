/*
 * SPI.h
 *
 *  Created on: Oct 18, 2025
 *      Author: A&M
 */

#ifndef SPI_SPI_H_
#define SPI_SPI_H_

#include "stm32f103xx.h"
#include "gpio.h"

typedef struct
{
	uint8_t TXE:1;				// TX buffer empty interrupt
	uint8_t RXNE:1;				// RX buffer not empty interrupt
	uint8_t ERRI:1;				// error interrupt
	uint8_t Reserved:5;

}SPI_IRQ_SRC_t;
typedef struct
{
	uint16_t SPI_Mode;					 // MASTER VS SLAVE
	uint16_t SPI_Communication_Mode;	 // RX Only, RX & TX
	uint16_t SPI_Data_Frame_Format;      // 8 OR 16 BIT
	uint16_t SPI_MASTER_CLK_PRESCALER;	 // FREQ / 2 ,4,8,16,...
	uint16_t SPI_NSS_Mangement;      	 // HW OR SW
	uint16_t SPI_CLK_Idle;        	 	// HIGH OR LOW
	uint16_t SPI_Sampling;     	   	 	// First transition or second transition
	uint16_t SPI_First_Bit;          	 // LSB OR MSB
	uint16_t SPI_Interrupt;				// Disable or enable one of interrupt modes
	SPIx_typedef* SPIx;             	 // where x can be 1 or 2

	void (*SPIx_IRQ_CallBack)(SPI_IRQ_SRC_t src);
}SPIx_Config_t;


#define SPI_Mode_Master					(0x1U << 2)
#define SPI_Mode_Slave					0x00000000


#define SPI_8_Frame_Format				0x00000000
#define SPI_16_Frame_Format				(0x1U << 11)


#define SPI_RX_TX_2LINE					 0x00000000
#define SPI_RX_2LINE					(0x1U << 10)


#define SPI_1LINE_RX_Only				(0x1U << 15)
#define SPI_1LINE_TX_Only				(0x1U << 15) | (0x1U << 14)


#define SPI_NSS_SW										(0x1U << 9)
#define SPI_NSS_HW_OUTPUT_ENABLE						0x00000000
#define SPI_NSS_HW_OUTPUT_DISABLE						0x00000000


#define SPI_CLK_POL_Idle_HIGH				(0x1 << 1)
#define SPI_CLK_POL_Idle_LOW				0x00000000


#define SPI_PHA_Sample_first_transition			0x00000000
#define SPI_PHA_Sample_second_transition		(0x1U << 0)


#define SPI_MSB_FIRST					0x00000000
#define SPI_LSB_FIRST					(0x1 << 7)


#define SPIx_M_CLK_2			(0x00000000UL)
#define SPIx_M_CLK_4			(0x1 <<  3)
#define SPIx_M_CLK_8			(0x2 <<  3)
#define SPIx_M_CLK_16			(0x3 <<  3)
#define SPIx_M_CLK_32			(0x4 <<  3)
#define SPIx_M_CLK_64			(0x5 <<  3)
#define SPIx_M_CLK_128			(0x6 <<  3)
#define SPIx_M_CLK_256			(0x7 <<  3)

// SPI Interrupt

#define SPI_Interrupt_Disable				0x0000

#define SPI_TX_Interrupt_Enable				(0x1 << 7)
#define SPI_RX_Interrupt_Enable				(0x1 << 6)
#define SPI_ERROR_Interrupt_Enable			(0x1 << 5)




void SPIx_Init(SPIx_Config_t* SPIx_Config);

void SPIx_Send_Data(SPIx_typedef* SPIx , uint16_t *data, uint16_t SPI_Interrupt);

uint16_t SPIx_ReceiveData(SPIx_typedef* SPIx , uint16_t SPI_Interrupt);

void SPIx_Send_Receive(SPIx_typedef* SPIx , uint16_t *sdata, uint16_t* rdata, uint16_t SPI_Interrupt);


#endif /* SPI_SPI_H_ */
