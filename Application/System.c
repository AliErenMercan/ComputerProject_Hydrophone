/*
 * System.c
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */




#include "System.h"
#include "IMU/AEM_IMU.h"

_Tasks Tasks = {0};

void SysTimerUpdate(){
	Tasks.IO++;
	Tasks.BNO055++;
	IMU_DELAY_COUNTER++;
}
