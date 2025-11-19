/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Abeer Sobhy
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stdint.h>
#include "App.h"

int main(void)
{
	App_Init();
	while(1)
	{
		App_Start();
	}
}
