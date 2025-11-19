/*
 * RCC.h
 *
 *  Created on: Oct 14, 2025
 *      Author: A&M
 */

#include "stm32f103xx.h"

#ifndef RCC_RCC_H_
#define RCC_RCC_H_

#define HSI_CLK	(uint32_t)8000000
#define HSE_CLK	(uint32_t)16000000

uint32_t RCC_GetPCLK1Freq(void);

uint32_t RCC_GetPCLK2Freq(void);

#endif /* RCC_RCC_H_ */
