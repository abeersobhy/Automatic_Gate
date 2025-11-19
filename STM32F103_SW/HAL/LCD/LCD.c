/*
 *  LCD.c
 *  Author: Abeer Sobhy
 */

#include "stdlib.h"
#include "stdio.h"

#include "LCD.h"

void LCD_Init(void)
{

	GPIO_Pin_Config D7 = {GPIOx_PIN_12 , GPIO_MODE_OUTPUT_PP, GPIO_SPEED_10MHZ};
	GPIO_Pin_Config D6 = {GPIOx_PIN_13 , GPIO_MODE_OUTPUT_PP, GPIO_SPEED_10MHZ};
	GPIO_Pin_Config D5 = {GPIOx_PIN_14 , GPIO_MODE_OUTPUT_PP, GPIO_SPEED_10MHZ};
	GPIO_Pin_Config D4 = {GPIOx_PIN_15 , GPIO_MODE_OUTPUT_PP, GPIO_SPEED_10MHZ};
	GPIO_Pin_Config RS = {GPIOx_PIN_11 , GPIO_MODE_OUTPUT_PP, GPIO_SPEED_10MHZ};
	GPIO_Pin_Config EN = {GPIOx_PIN_8  , GPIO_MODE_OUTPUT_PP, GPIO_SPEED_10MHZ};

	MCAL_GPIO_Init(GPIOB , &D7);
	MCAL_GPIO_Init(GPIOB , &D6);
	MCAL_GPIO_Init(GPIOB , &D5);
	MCAL_GPIO_Init(GPIOB , &D4);
	MCAL_GPIO_Init(GPIOA , &RS);
	MCAL_GPIO_Init(GPIOA , &EN);


	LCD_Write_Command(0x3);
	delay_ms(4);
	LCD_Write_Command(0x3);
	delay_ms(4);
	LCD_Write_Command(0x3);
	delay_ms(4);
	LCD_Write_Command(MOOD4);
	LCD_Write_Command(LCD_4BIT_2LINE);
	LCD_Write_Command(DISPLAY_OFF_CURSER_OFF);
	LCD_Write_Command(LCD_Clear);
	LCD_Write_Command(LCD_CURSER_INC_RIGHT);
	LCD_Write_Command(LCD_Clear);
	LCD_Write_Command(LCD_TurnOnDisplay);
}
void LCD_Write_Command(uint8_t cmd)
{
     RS(0);/*Select Command register.*/
	 D7(GET_BIT(cmd,7)); 
	 D6(GET_BIT(cmd,6));    /*Send the most four bit to data pins.*/
	 D5(GET_BIT(cmd,5));
	 D4(GET_BIT(cmd,4)); 
	 TRIGGER_LCD(); /*Enable EN for 1 ms then disable it for 1ms.*/
	 D7(GET_BIT(cmd,3));
	 D6(GET_BIT(cmd,2));   /*Send the least four bit to data pins.*/
	 D5(GET_BIT(cmd,1));
	 D4(GET_BIT(cmd,0));
     TRIGGER_LCD(); /*Enable EN for 1 ms then disable it for 1ms.*/
}
void LCD_Write_Char(uint8_t data)
{
	RS(1);
	D7(GET_BIT(data,7));
	D6(GET_BIT(data,6));    /*Send the most four bit to data pins.*/
	D5(GET_BIT(data,5));
	D4(GET_BIT(data,4));
	TRIGGER_LCD(); /*Enable EN for 1 ms then disable it for 1ms.*/
	D7(GET_BIT(data,3));
	D6(GET_BIT(data,2));   /*Send the least four bit to data pins.*/
	D5(GET_BIT(data,1));
	D4(GET_BIT(data,0));
	TRIGGER_LCD(); /*Enable EN for 1 ms then disable it for 1ms.*/
}
void LCD_Write_String(uint8_t *txt)
{
     uint8_t index = 0;
	 while(txt[index] != '\0')
	 {
		 LCD_Write_Char(txt[index]);
		 index++;
	 }
}
void LCD_Write_Number(int32_t number)
{
	if(number == 0)
	{
		LCD_Write_Char('0');
	}
	else if(number > 0)
	{
		uint8_t arr_num[10] = {0};
		int8_t i = 0;
		while(number > 0)
		{
			arr_num[i] = (number % 10) + 48;
			number /= 10;
			i++;
		}
		for(int8_t j = (i-1) ; j >= 0 ; j--)
		{
			LCD_Write_Char(arr_num[j]);
		}
	}
	else
	{
		number*= -1;
		uint8_t arr_num[10] = {0};
		int8_t i = 0;
		while(number > 0)
		{
			arr_num[i] = (number % 10) + 48;
			number /= 10;
			i++;
		}
		LCD_Write_Char('-');
		for(int8_t j = (i-1) ; j >= 0 ; j--)
		{
			LCD_Write_Char(arr_num[j]);
		}
	}
}
void LCD_Display_Number(double number)
{
	int32_t NUM_First = number;
	int32_t NUM_Second = (number - NUM_First)*100;
	uint8_t arr_num[10] = {0};
	uint8_t arr_num2[10] = {0};
	int8_t i = 0 , k = 0;
	/*Decimal number.*/
	if(number == 0)
	{
		LCD_Write_Char('0');
	}
	else if(number > 0) /*Psitive numbers.*/
	{
		while(NUM_First > 0)
		{
			arr_num[i] = (NUM_First % 10) + 48;
			NUM_First /= 10;
			i++;
		}
		for(int8_t j = (i-1) ; j >= 0 ; j--)
		{
			LCD_Write_Char(arr_num[j]);
		}
		if(NUM_Second != 0)
		{
			LCD_Write_Char('.');
		}
		/*Float number.*/
		while(NUM_Second > 0)
		{
			arr_num2[k] = (NUM_Second % 10) + 48; /* 255 % 10 = 5 */
			NUM_Second /= 10;
			k++;
		}
		for(int8_t j = (k-1) ; j >= 0 ; j--)
		{
			LCD_Write_Char(arr_num2[j]);
		}	
	}
	else /*Negative numbers.*/
	{
		NUM_First*= -1;
		while(NUM_First > 0)
		{
			arr_num[i] = (NUM_First % 10) + 48;
			NUM_First /= 10;
			i++;
		}
		LCD_Write_Char('-');
		for(int8_t j = (i-1) ; j >= 0 ; j--)
		{
			LCD_Write_Char(arr_num[j]);
		}
		if(NUM_Second != 0)
		{
			LCD_Write_Char('.');
		}
		/*Float number.*/
		while(NUM_Second > 0)
		{
			arr_num2[k] = (NUM_Second % 10) + 48; /* 255 % 10 = 5 */
			NUM_Second /= 10;
			k++;
		}
		for(int8_t j = (k-1) ; j >= 0 ; j--)
		{
			LCD_Write_Char(arr_num2[j]);
		}
	}
}
