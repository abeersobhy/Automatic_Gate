/*
 * App.h
 *
 *  Created on: Nov 18, 2025
 *      Author: A&M
 */

#ifndef APP_H_
#define APP_H_

#include "Ultrasonic.h"
#include "Servo.h"
#include "LCD.h"


#define SERVO_PIN					SERVO_GPIOA_PIN7

#define ULTRASONIC_ECHO_PIN			ECHO_GPIOA_PIN0
#define ULTRASONIC_TRIG_PIN			GPIOx_PIN_15

#define ULTRASONIC_ECHO_PORT		GPIOA
#define ULTRASONIC_TRIG_PORT		GPIOC

// -----------------------------------------------
//              Configuration
// -----------------------------------------------
#define OPEN_TH        15   // cm
#define CLOSE_TH	   18

#define DOOR_OPEN_ANGLE   90
#define DOOR_CLOSE_ANGLE  0

#define DOOR_SPEED_DELAY  30   // ms per step
#define REFRESH_RATE      200  // ms


void App_Init();
void App_Start();

#endif /* APP_H_ */
