/*
 * Servo.h
 *
 *  Created on: Nov 18, 2025
 *  Author: Abeer Sobhy
 */

#ifndef SERVO_SERVO_H_
#define SERVO_SERVO_H_

#include "Timer.h"

#define SERVO_GPIOA_PIN1	0
#define SERVO_GPIOA_PIN7	1
#define SERVO_GPIOB_PIN7	2

/*
 * Where Servo pin can be SERVO_GPIOA_PIN1, SERVO_GPIOA_PIN7, OR SERVO_GPIOB_PIN7
 */
void Servo_Attach(uint8_t ServoPin);
/*
 * Where angle is any number between 0 to 180.
 */
void Servo_Write(uint8_t angle);

#endif /* SERVO_SERVO_H_ */
