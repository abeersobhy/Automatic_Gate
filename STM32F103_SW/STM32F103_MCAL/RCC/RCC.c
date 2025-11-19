/*
 * RCC.c
 *
 *  Created on: Oct 14, 2025
 *      Author: A&M
 */

#include "RCC.h"

uint8_t APB_Prescaler[8] = {0,0,0,0,1,2,3,4};
uint8_t AHB_Prescaler[16] = {0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9};

uint32_t RCC_GetSysCLK(void)
{
	switch((RCC->CFGR >> 2) & 0b00) /*System clock switch status*/
	{
	case 0:
		return HSI_CLK;
		break;
	case 1:
		//todo
		return HSE_CLK;
		break;
	case 2:
		//todo
		return 16000000;
		break;
	}
}
uint32_t RCC_GetHCLKFreq(void)
{
	uint8_t Prescaler = ((RCC->CFGR >> 4)& 0xF);
	return (RCC_GetSysCLK() >> AHB_Prescaler[Prescaler]);
}
uint32_t RCC_GetPCLK1Freq(void)
{
	uint8_t Prescaler = ((RCC->CFGR >> 8)& 0x7);
	return (RCC_GetHCLKFreq() >> APB_Prescaler[Prescaler]);
}
uint32_t RCC_GetPCLK2Freq(void)
{
	uint8_t Prescaler = ((RCC->CFGR >> 11)& 0x7);
	return (RCC_GetHCLKFreq() >> APB_Prescaler[Prescaler]);
}
