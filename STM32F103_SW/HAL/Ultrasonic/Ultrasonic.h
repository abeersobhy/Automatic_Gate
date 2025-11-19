/*
 * Ultrasonic.h
 *
 *  Created on: Nov 17, 2025
 *      Author: Abeer Sobhy
 */

#ifndef ULTRASONIC_ULTRASONIC_H_
#define ULTRASONIC_ULTRASONIC_H_

#include "Timer.h"

#define ECHO_GPIOA_PIN0		0
#define ECHO_GPIOA_PIN6		1
#define ECHO_GPIOB_PIN6		2

typedef struct
{
	uint16_t echo_pin;  // ECHO_GPIOA_PIN6 OR ECHO_GPIOB_PIN6
	GPIOx_typedef* echo_port; // GPIOA or GPIOB
	uint16_t trig_pin;  // any GPIO Pin
	GPIOx_typedef* trig_port; // GPIOA , GPIOB , OR GPIOC
}Ultrasonic_Init_t;

void Ultrasonic_Init(Ultrasonic_Init_t* ultrasonic_init);

double Ultrasonic_GetDistance();


#endif /* ULTRASONIC_ULTRASONIC_H_ */
