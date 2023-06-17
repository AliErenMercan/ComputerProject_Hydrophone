/*
 * Application.c
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */


#include "IMU/AEM_IMU.h"
#include "Application.h"
#include "System.h"
#include "InputOutput/InputOutput.h"
#include "Communication/FK_Uart.h"

uint8_t txBuffer[5] = "OK\n";

void Application(){

	AEM_IMU_Init();
	UartInit();
	AudioSignalControlInit();

	while(1){

		if(Tasks.IO > IO_TASK_RATE){
			Tasks.IO = 0;

			USERLED = (IMU_ONCE_READ == IMU_ONCE_READED) ? USERLED ^ 1 : 0;
			OutputProcess();

			HAL_UART_Transmit_IT(&huart1, txBuffer, 5);
		}

		if(Tasks.BNO055 > BNO055_TASK_RATE){
			Tasks.BNO055 = 0;
			IMU_Process();
		}

		if(Tasks.HYDROPHONE > HP_TASK_RATE)
		{
			Tasks.HYDROPHONE = 0;
			AudioSignalProcess();
		}
	}
}
