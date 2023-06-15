/*
 * InputOutput.c
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */


#include "InputOutput.h"

_Outputs Outputs = {0};


void OutputProcess(){
	HAL_GPIO_WritePin(USERLED_PORT, USERLED_PIN, USERLED);
}
