/*
 * SPI.c
 *
 *  Created on: Oct 18, 2025
 *  Author: A&M
 */


#include "SPI.h"

//Array of 2 Pointers to function
void (*SPIx_IRQ_CallBack[2])(SPI_IRQ_SRC_t src);


void SPIx_Init(SPIx_Config_t* SPIx_Config)
{
	uint16_t temp_CR1 = 0;
	uint16_t temp_CR2 = 0;

	GPIO_Pin_Config SCK;
	GPIO_Pin_Config MOSI;
	GPIO_Pin_Config MISO;
	GPIO_Pin_Config NSS;

	/*Check SPI number 1 OR 2.*/
	if(SPIx_Config->SPIx == SPI1)
	{
		/*Enable CLK for GPIOA.*/
		RCC_GPIOA_CLK_EN();
		/*Enable CLK for AFIO.*/
		RCC_AFIO_CLK_EN();
		/*Enable SPI1 CLK*/
		RCC_SPI1_CLK_EN();

		if(temp_CR1 |= SPIx_Config->SPI_Mode == SPI_Mode_Master)
		{
			SCK.GPIO_PinNumber = GPIOx_PIN_5;
			SCK.GPIO_PinMode = GPIO_MODE_AFIO_PP;       //FOR MASTER

			MOSI.GPIO_PinNumber = GPIOx_PIN_7;
			MOSI.GPIO_PinMode = GPIO_MODE_AFIO_PP;//FOR MASTER

			MISO.GPIO_PinNumber = GPIOx_PIN_6;
			MISO.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;//FOR MASTER

			if(SPIx_Config->SPI_NSS_Mangement != SPI_NSS_SW)
			{
				NSS.GPIO_PinNumber = GPIOx_PIN_4;
				NSS.GPIO_PinMode = GPIO_MODE_AFIO_PP;//FOR MASTER
			}

		}
		else if(temp_CR1 |= SPIx_Config->SPI_Mode == SPI_Mode_Slave)
		{
			SCK.GPIO_PinNumber = GPIOx_PIN_5;
			SCK.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;//FOR SLAVE

			MOSI.GPIO_PinNumber = GPIOx_PIN_7;
			MOSI.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;//FOR SLAVE

			MISO.GPIO_PinNumber = GPIOx_PIN_6;
			MISO.GPIO_PinMode = GPIO_MODE_AFIO_PP;//FOR SLAVE
			if(SPIx_Config->SPI_NSS_Mangement != SPI_NSS_SW)
			{
				NSS.GPIO_PinNumber = GPIOx_PIN_4;
				NSS.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;//FOR SLAVE
			}
		}
		MCAL_GPIO_Init(GPIOA , &SCK);
		MCAL_GPIO_Init(GPIOA , &NSS);
		MCAL_GPIO_Init(GPIOA , &MISO);
		MCAL_GPIO_Init(GPIOA , &MOSI);
	}
	else if(SPIx_Config->SPIx == SPI2)
	{
		/*Enable CLK for GPIOB.*/
		RCC_GPIOB_CLK_EN();
		/*Enable CLK for AFIO.*/
		RCC_AFIO_CLK_EN();
		/*Enable SPI1 CLK*/
		RCC_SPI2_CLK_EN();

		if(temp_CR1 |= SPIx_Config->SPI_Mode == SPI_Mode_Master)
		{
			SCK.GPIO_PinNumber = GPIOx_PIN_13;
			SCK.GPIO_PinMode = GPIO_MODE_AFIO_PP;       //FOR MASTER

			MOSI.GPIO_PinNumber = GPIOx_PIN_15;
			MOSI.GPIO_PinMode = GPIO_MODE_AFIO_PP;//FOR MASTER

			MISO.GPIO_PinNumber = GPIOx_PIN_14;
			MISO.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;//FOR MASTER

			if(SPIx_Config->SPI_NSS_Mangement != SPI_NSS_SW)
			{
				NSS.GPIO_PinNumber = GPIOx_PIN_12;
				NSS.GPIO_PinMode = GPIO_MODE_AFIO_PP;//FOR MASTER
			}
		}
		else if(temp_CR1 |= SPIx_Config->SPI_Mode == SPI_Mode_Slave)
		{
			SCK.GPIO_PinNumber = GPIOx_PIN_13;
			SCK.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;//FOR SLAVE

			MOSI.GPIO_PinNumber = GPIOx_PIN_15;
			MOSI.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;//FOR SLAVE

			MISO.GPIO_PinNumber = GPIOx_PIN_14;
			MISO.GPIO_PinMode = GPIO_MODE_AFIO_PP;//FOR SLAVE

			if(SPIx_Config->SPI_NSS_Mangement != SPI_NSS_SW)
			{
				NSS.GPIO_PinNumber = GPIOx_PIN_12;
				NSS.GPIO_PinMode = GPIO_MODE_INPUT_FLOATING;//FOR SLAVE
			}
		}
		MCAL_GPIO_Init(GPIOB , &SCK);
		MCAL_GPIO_Init(GPIOB , &NSS);
		MCAL_GPIO_Init(GPIOB , &MISO);
		MCAL_GPIO_Init(GPIOB , &MOSI);
	}


	/*SPI Mode - Master OR Slave*/
	temp_CR1 &= ~(1 << 2); //RESET MASTER BIT
	temp_CR1 |= SPIx_Config->SPI_Mode;

	/*SPI Communication Mode*/
	temp_CR1 |= SPIx_Config->SPI_Communication_Mode;

	/*Data frame format - 8 or 16 bit*/
	temp_CR1 |= SPIx_Config->SPI_Data_Frame_Format;

	/*Data first bit - LSB , MSB*/
	temp_CR1 |= SPIx_Config->SPI_First_Bit;

	/*Data CLK Polarity*/
	temp_CR1 &= ~(1 << 1); //POL BIT RESET
	temp_CR1 |= SPIx_Config->SPI_CLK_Idle;

	/*Data CLK PHA*/
	temp_CR1 &= ~(1 << 0); //PHA BIT RESET
	temp_CR1 |= SPIx_Config->SPI_Sampling;

	/*BAUDRATE*/
	temp_CR1 &= ~(0b111 << 3); //reset baud rate bits
	temp_CR1 |= SPIx_Config->SPI_MASTER_CLK_PRESCALER;

	/*NSS*/
	if(SPIx_Config->SPI_NSS_Mangement == SPI_NSS_HW_OUTPUT_ENABLE && SPIx_Config->SPI_Mode == SPI_Mode_Master)
	{
		/*
		 * NSS  - HW - SSOE is set, SS output is enabled in master mode and when the cell is enabled. The cell cannot work
						  in a multimaster environment.
		 * Mode - Master
		 */
		temp_CR2 |= (1 << 2);
		if(SPIx_Config->SPIx == SPI1)
		{
			MCAL_GPIO_WritePin(GPIOA, GPIOx_PIN_4, TRUE);
		}
		else if(SPIx_Config->SPIx == SPI2)
		{
			MCAL_GPIO_WritePin(GPIOB, GPIOx_PIN_12, TRUE);
		}
	}
	else if (SPIx_Config->SPI_NSS_Mangement == SPI_NSS_SW)
	{
		temp_CR1 |= (1 << 9); // SSM - Software slave management enabled
		temp_CR1 |= (1 << 8); // SSI: Internal slave select
	}

	/*IRQ*/
	if(SPIx_Config->SPI_Interrupt != SPI_Interrupt_Disable)
	{
		temp_CR2 |= SPIx_Config->SPI_Interrupt;
		if(SPIx_Config->SPIx == SPI1)
		{
			NVIC_SPI1_Enable;
			SPIx_IRQ_CallBack[0] = SPIx_Config->SPIx_IRQ_CallBack;
		}
		else if(SPIx_Config->SPIx == SPI2)
		{
			NVIC_SPI2_Enable;
			SPIx_IRQ_CallBack[1] = SPIx_Config->SPIx_IRQ_CallBack;
		}
		else{}
	}
	else{}

	SPIx_Config->SPIx->SPI_CR1 = temp_CR1;

	/*Enable the specified SPI*/
	SPIx_Config->SPIx->SPI_CR1 |= (0x1 << 6);


	SPIx_Config->SPIx->SPI_CR2 = temp_CR2;
}
void SPIx_Send_Data(SPIx_typedef* SPIx , uint16_t *data, uint16_t SPI_Interrupt)
{
	/*Full duplex mode*/
	if(SPI_Interrupt == SPI_Interrupt_Disable)
	{
		while(!((SPIx->SPI_SR >> 1) & 0x1));
	}
	SPIx->SPI_DR = *data;
}
uint16_t SPIx_ReceiveData(SPIx_typedef* SPIx , uint16_t SPI_Interrupt)
{
	/*Full duplex mode*/
	if(SPI_Interrupt == SPI_Interrupt_Disable)
	{
		while(!((SPIx->SPI_SR >> 0) & 0x1));
	}
	return SPIx->SPI_DR;
}
void SPIx_Send_Receive(SPIx_typedef* SPIx , uint16_t *sdata, uint16_t* rdata, uint16_t SPI_Interrupt)
{
	while(SPIx->SPI_SR & (1 << 7));  // Busy flag

	if(SPI_Interrupt == SPI_Interrupt_Disable)
	{
		while(!((SPIx->SPI_SR >> 1) & 0x1));
	}
	SPIx->SPI_DR = *sdata;

	if(SPI_Interrupt == SPI_Interrupt_Disable)
	{
			while(!((SPIx->SPI_SR >> 0) & 0x1));
	}
	*rdata = SPIx->SPI_DR;

	while(SPIx->SPI_SR & (1 << 7));
}

/*IRQ.*/

void SPI1_IRQHandler(void)
{
	SPI_IRQ_SRC_t src;
	src.TXE = ((SPI1->SPI_SR >> 1) & 1);
	src.RXNE = ((SPI1->SPI_SR >> 0) & 1);
	src.TXE = ((SPI1->SPI_SR >> 4) & 1);

	SPIx_IRQ_CallBack[0](src);
}

void SPI2_IRQHandler(void)
{
	SPI_IRQ_SRC_t src;
	src.TXE = ((SPI2->SPI_SR >> 1) & 1);
	src.RXNE = ((SPI2->SPI_SR >> 0) & 1);
	src.TXE = ((SPI2->SPI_SR >> 4) & 1);

	SPIx_IRQ_CallBack[1](src);
}
