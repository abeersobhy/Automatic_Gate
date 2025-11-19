/*
 * I2C.c
 *
 *  Created on: Nov 6, 2025
 *  Author: Abeer Sobhy
 */

#include "I2C.h"

/*********************Functions declaration**********************.*/
void I2C_GenerateStart(I2Cx_typedef* I2Cx,FunctionalState_e FS ,RepeatedStart_e RS);
/******************************************************************/

void I2Cx_Init(I2C_Init_t* I2C_Init)
{
	uint32_t temp = 0;
	double CCR = 0;
	uint32_t PCLK = 8000000;
	double T_high = 0;
	double T_PCLK1 =0;

	GPIO_Pin_Config SCL;
	GPIO_Pin_Config SDA;

	if(I2C_Init->I2Cx == I2C1)
	{
		/*Enable CLK for GPIOB.*/
		RCC_GPIOB_CLK_EN();

		/*Enable CLK for AFIO.*/
		RCC_AFIO_CLK_EN();

		/*Enable CLK of I2C.*/
		RCC_I2C1_CLK_EN();


		SCL.GPIO_PinNumber = GPIOx_PIN_6;			//PB6
		SCL.GPIO_PinMode = GPIO_MODE_AFIO_OD;       //Open drain
		SCL.GPIO_PinSpeed = GPIO_SPEED_10MHZ;

		SDA.GPIO_PinNumber = GPIOx_PIN_7;			//PB7
		SDA.GPIO_PinMode = GPIO_MODE_AFIO_OD;       //Open drain
		SDA.GPIO_PinSpeed = GPIO_SPEED_10MHZ;

		MCAL_GPIO_Init(GPIOB , &SCL);
		MCAL_GPIO_Init(GPIOB , &SDA);


	}
	else
	{
		/*Enable CLK for GPIOB.*/
		RCC_GPIOB_CLK_EN();

		/*Enable CLK for AFIO.*/
		RCC_AFIO_CLK_EN();

		/*Enable CLK of I2C.*/
		RCC_I2C2_CLK_EN();

		SCL.GPIO_PinNumber = GPIOx_PIN_10;			//PB10
		SCL.GPIO_PinMode = GPIO_MODE_AFIO_OD;       //Open drain
		SCL.GPIO_PinSpeed = GPIO_SPEED_10MHZ;

		SDA.GPIO_PinNumber = GPIOx_PIN_11;			//PB11
		SDA.GPIO_PinMode = GPIO_MODE_AFIO_OD;       //Open drain
		SDA.GPIO_PinSpeed = GPIO_SPEED_10MHZ;

		MCAL_GPIO_Init(GPIOB , &SCL);
		MCAL_GPIO_Init(GPIOB , &SDA);
	}

	if(I2C_Init->I2C_Mode == I2C_Mode_I2C)
	{
		/*======================Init Timing=================.*/
		/*Set Peripheral Clock Frequency ==> 2MHz : 50MHz supported */
		temp = I2C_Init->I2Cx->I2C_CR2;

		/*Clear frequency bits.*/
		temp &= ~(I2C_CR2_FREQ_Msk);
		PCLK = RCC_GetPCLK1Freq();

		/*Get the frequency in Mega.*/
		PCLK /= 1000000;
		temp |= (uint16_t)(PCLK);

		I2C_Init->I2Cx->I2C_CR2 |= temp;

		/*Make sure the selected I2C is disable.*/
		I2C_Init->I2Cx->I2C_CR1 &= ~(I2C_CR1_PE);
		temp = 0;
		/*Configure the clock control registers*/
		if(I2C_Init->I2C_SM_OR_FM == I2C_SM_Enable)
		{
			/* T-high  = CCR * T-PCLK1
			 * T-high  = (1 / RequiredFreq) / 2.
			 * T-PLCK1 = (1 / PCLK*1000000)
			 * CRR = T-high / T-PCLK1
			 * */
			T_high = (1.0 / (I2C_Init->I2C_ClockSpeed)) / 2.0;
			T_PCLK1 = 1.0 / (PCLK*1000000);
			CCR   = (T_high / T_PCLK1);
			temp |= (uint16_t)CCR;
			I2C_Init->I2Cx->I2C_CCR |= temp;
			/*Configure the T rising.*/
			I2C_Init->I2Cx->I2C_TRISE |= (I2C_Init->I2Cx->I2C_CR2 & (0x001F)) + 1;
		}
		else
		{
			/*Fast Mode Enable.*/
			/*
			 * If DUTY = 0
			 * T-high  = CCR * TPCLK1
			 * T-low   = 2 * CCR * TPCLK1
			 * *******
			 * T_i2c_CLK = 1 / (I2C_Init->I2C_ClockSpeed)
			 * T_i2c_CLK = 2 * T-high + T-high
			 * 1 / (I2C_Init->I2C_ClockSpeed) = 3 * T-high
			 * T-high = 3 * I2C_Init->I2C_ClockSpeed
			 * CCR = T-high / PCLK1
			 * CCR =  (3 * I2C_Init->I2C_ClockSpeed) / PCLK1
			 * .*/
			CCR   = (3.0 * I2C_Init->I2C_ClockSpeed) / (PCLK*1000000);

			temp |= (uint16_t)CCR;

			I2C_Init->I2Cx->I2C_CCR |= temp;

		}
		temp = I2C_Init->I2Cx->I2C_CR1;
		/* 	- Program the I2C_CR1 register to enable the peripheral
	 	 	- Set the START bit in the I2C_CR1 register to generate a Start condition*/
		// 	Ack Control, Stretching Control , General Call Addressing Control
		temp |= (I2C_Init->I2C_ACK_Control | I2C_Init->I2C_StretchMode | I2C_Init->I2C_GeneralCall_Adressing);
		I2C_Init->I2Cx->I2C_CR1 = temp;
		/*********I2C address register (I2C_OAR1) & (I2C_OAR2)************/
		if(I2C_Init->slaveAddressConfig.Addressing_Slave_Mode  == DualAddress_Enable)
		{
			temp = I2C_OAR2_ENDUAL; // Enable Dual Addressing
			temp |= (I2C_Init->slaveAddressConfig.Slave_SecondaryAddress << 1);	// Set the secondary slave address
			I2C_Init->I2Cx->I2C_OAR2 = temp;
		}
		temp = 0;

		temp |= (I2C_Init->slaveAddressConfig.Slave_PrimaryAddress << 1);
		/*7 Bit or 10 Bit address*/
		temp |= (I2C_Init->slaveAddressConfig.Addressing_Slave_Mode);
		I2C_Init->I2Cx->I2C_OAR1 = temp;

		/*Master or slave.*/
		/* Polling active with master.
		 * Interrupt active with slave.*/
		if(I2C_Init->I2C_Master_Slave_Mode == I2C_Slave_Enable)
		{
			// Enable IRQ
			I2C_Init->I2Cx->I2C_CR2 |= (I2C_CR2_ITBUFEN | I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);
			if(I2C_Init->I2Cx == I2C1)
			{
				NVIC_I2C1_ER_Enable;
				NVIC_I2C1_EV_Enable;
			}
			else
			{
				NVIC_I2C2_ER_Enable;
				NVIC_I2C2_EV_Enable;
			}
		}
		/*Reset the Status Registers.*/
		I2C_Init->I2Cx->I2C_SR1 = 0;
		I2C_Init->I2Cx->I2C_SR2 = 0;

		/*Enable the selected I2C.*/
		I2C_Init->I2Cx->I2C_CR1 |= (I2C_CR1_PE);
	}
	else
	{
		// I2C_Mode_SMBus NOT SUPPORTED HERE
	}
}

void I2C_Master_TX(I2Cx_typedef* I2Cx, const I2C_Device_Address* slave_address, uint8_t* dataOut, uint32_t dataLen, Stop_Condition_e st, RepeatedStart_e RS)
{
	int i = 0;
	uint16_t address = 0;
	volatile uint32_t StatusRegisters = 0;
	// Send Start Condition
	I2C_GenerateStart(I2Cx, Start_Enable, Start);
	// Wait until the bus become idle
	while(!(I2Cx->I2C_SR1 & I2C_SR1_SB));
	// Send Address
	if(slave_address->Addressing_Slave_Mode == Addressing_Slave_Mode_7BIT)
	{
		address |= (uint16_t)(slave_address->Slave_PrimaryAddress << 1);
		address &= ~(1 << 0); // reset the first bit for writing
		I2Cx->I2C_DR = address;
		// Wait until the address matching bit is set
		while(!(I2Cx->I2C_SR1 & I2C_SR1_ADDR));
		/*Check data is transmitted, The Master still hold the bus, Bus busy*/

		StatusRegisters  = I2Cx->I2C_SR2;
		StatusRegisters  = (StatusRegisters << 16);
		StatusRegisters |= I2Cx->I2C_SR1;

		while(!((StatusRegisters & I2C_EVENT_MASTER_BYTE_TRANSMITTING) == I2C_EVENT_MASTER_BYTE_TRANSMITTING));

		/*Send data.*/
		for(i = 0; i < dataLen; i++)
		{
			I2Cx->I2C_DR = dataOut[i];
			while(!(I2Cx->I2C_SR1 & I2C_SR1_TXE)); // Wait until data transmitted
		}
		/*Generate Stop Condition.*/
		if(st == Stop)
		{
			I2Cx->I2C_CR1 |= (I2C_CR1_STOP);
		}
		else
		{
			I2Cx->I2C_CR1 &= ~(I2C_CR1_STOP);
		}
	}
	else{ /* 10 Bit address mode is not supported */}
}
void I2C_Master_RX(I2Cx_typedef* I2Cx, const I2C_Device_Address* slave_address, uint8_t* dataOut, uint32_t dataLen, Stop_Condition_e st, RepeatedStart_e RS)
{
	// To Do : Configure Time Out
	int i = 0; uint16_t address = 0;
	// Send Start Condition
	I2C_GenerateStart(I2Cx, Start_Enable, Start);
	// Wait until the bus become idle
	while(!(I2Cx->I2C_SR1 & I2C_SR1_SB));
	// Send Address
	if(slave_address->Addressing_Slave_Mode == Addressing_Slave_Mode_7BIT)
	{
		address |= (uint16_t)(slave_address->Slave_PrimaryAddress << 1);
		address |= (1 << 0);
		// set the first bit for Reading
		I2Cx->I2C_DR = address;
		// Wait until the address matching bit is set
		while(!(I2Cx->I2C_SR1 & I2C_SR1_ADDR));

		// Clear ADDR flag by reading SR1 then SR2
		(void)I2Cx->I2C_SR1;
		(void)I2Cx->I2C_SR2;

	    if(dataLen == 1)
	    {
	        // 1 byte: NACK + STOP before reading
	        I2Cx->I2C_CR1 &= ~I2C_CR1_ACK;   // NACK
	        if(st == Stop)
	        {
	        	I2Cx->I2C_CR1 |= I2C_CR1_STOP; // Prepare stop & Send it after receiving this byte
	        }
	        while(!(I2Cx->I2C_SR1 & I2C_SR1_RXNE)); // First and Last Byte
	        *dataOut = I2Cx->I2C_DR; // Read byte
	    }
	    else if(dataLen == 2)
	    {
	    	/* POS = 0 is sufficient for most practical cases on the STM32F103 if:
	    	 * You disable ACK before the first byte
	    	 * STOP is prepared before the last byte
	    	 * POS=1 is only important for edge cases or if you want to strictly follow the official Reference Manual.
			*/
	        I2Cx->I2C_CR1 &= ~I2C_CR1_ACK;   // NACK
	        if(st == Stop)
	        {
	        	I2Cx->I2C_CR1 |= I2C_CR1_STOP;
	        }
	        // wait for first byte
	        while(!(I2Cx->I2C_SR1 & I2C_SR1_RXNE));
	        *dataOut++ = I2Cx->I2C_DR;

	        // wait for second byte
	        while(!(I2Cx->I2C_SR1 & I2C_SR1_RXNE));
	        *dataOut++ = I2Cx->I2C_DR;
	    }
	    else
	    {
	        // bytes > 2: ACK enabled for all but last byte
	        for(i = 0; i < dataLen; i++)
	        {
	            if(i == dataLen - 1) // last byte
	            {
	                I2Cx->I2C_CR1 &= ~I2C_CR1_ACK;  // NACK
	                if(st == Stop) I2Cx->I2C_CR1 |= I2C_CR1_STOP;
	            }
	            while(!(I2Cx->I2C_SR1 & I2C_SR1_RXNE));
	            *dataOut++ = I2Cx->I2C_DR;
	        }
	    }
	    // Restore ACK for next reception
	    I2Cx->I2C_CR1 |= I2C_CR1_ACK;
	}
	else{/* 10 Bit address mode is not supported */}
}
void I2C_GenerateStart(I2Cx_typedef* I2Cx,FunctionalState_e FS ,RepeatedStart_e RS)
{
	I2Cx->I2C_CR1 &= ~(I2C_CR1_STOP);  // Make sure the stop bit is reset

	if(RS != RepeatedStart)
	{
		// Check if the Bus is idel or not
		// Bit 1 - Status register 1
		while(I2Cx->I2C_SR2 & I2C_SR2_BUSY); // Wait until the bus become Idle
	}
	if(FS == Start_Enable)
	{
		I2Cx->I2C_CR1 |= (1 << 8); // Start Condition Generated
	}
	else
	{
		I2Cx->I2C_CR1 &= ~(I2C_CR1_START); // No Start Condition Generated
	}
}
