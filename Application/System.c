/*
 * System.c
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */




#include "System.h"
#include "IMU/AEM_IMU.h"
#include "AudioSignal_Hydrophone/AudioSignal_Hydrophone.h"

_Tasks Tasks = {0};

void SysTimerUpdate(){
	Tasks.IO++;
	Tasks.BNO055++;
	Tasks.HYDROPHONE++;
	IMU_DELAY_COUNTER++;
	if(HP_DELAY_COUNTER){HP_DELAY_COUNTER--;}
}
