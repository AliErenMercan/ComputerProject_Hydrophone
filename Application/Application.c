/*
 * Application.c
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */


#include "AEM_IMU.h"
#include "Application.h"
#include "System.h"
#include "InputOutput.h"


void Application(){

	AEM_IMU_Init();

	while(1){

		if(Tasks.IO > IO_TASK_RATE){
			Tasks.IO = 0;

			USERLED = (IMU_ONCE_READ == IMU_ONCE_READED) ? USERLED ^ 1 : 0;
			OutputProcess();
		}

		if(Tasks.BNO055 > BNO055_TASK_RATE){
			Tasks.BNO055 = 0;
			IMU_Process();
		}
	}
}
