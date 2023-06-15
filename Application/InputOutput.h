/*
 * InputOutput.h
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */

#ifndef INPUTOUTPUT_H_
#define INPUTOUTPUT_H_

#include "main.h"


typedef struct{
	uint8_t userLed;
}_Outputs;
extern _Outputs Outputs;

#define USERLED_PORT			GPIOB
#define USERLED_PIN				GPIO_PIN_3
#define USERLED					Outputs.userLed

void OutputProcess();

#endif /* INPUTOUTPUT_H_ */
