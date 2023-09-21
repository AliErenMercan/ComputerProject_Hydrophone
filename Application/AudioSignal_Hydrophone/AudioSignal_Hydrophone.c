/*
 * Application.c
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */

#include "AudioSignal_Hydrophone.h"
#include "../IMU/AEM_IMU.h"
#include "../Communication/FK_Uart.h"
AEM_AudioSignalControl_t AEM_AudioSignalControl = {0};

void AudioSignalControlInit()
{
	HAL_TIM_RegisterCallback(&HP_TIMER, HAL_TIM_PERIOD_ELAPSED_CB_ID, SignalPendingTimeoutCallback);

	HP_STATUS = HP_IDLE;
	HP_OLD_STATUS = HP_IDLE;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == HP_LEFT_EXTI_PIN)
	{
		LEFT_EXTI_Arrived();
	}
	else if(GPIO_Pin == HP_RIGHT_EXTI_PIN)
	{
		RIGHT_EXTI_Arrived();
	}
}

void SignalPendingTimeoutCallback()
{
	LL_EXTI_DisableIT_0_31(HP_RIGHT_EXTI_PIN);
	LL_EXTI_DisableIT_0_31(HP_LEFT_EXTI_PIN);
	HP_DELAY_COUNTER = 2000;
	HP_STATUS = HP_ERROR;
}

void LEFT_EXTI_Arrived()
{
	HP_LEFT_COME_COUNTER++;
	if(HP_STATUS == HP_IDLE)
	{
		LL_EXTI_DisableIT_0_31(HP_LEFT_EXTI_PIN);
		LL_EXTI_ClearFlag_0_31(HP_LEFT_EXTI_PIN);
		HAL_TIM_Base_Start_IT(&HP_TIMER);
		HP_FIRST_COME = LEFT_SIGNAL;
		HP_STATUS = HP_PENDING;
	}
	else if(HP_STATUS == HP_PENDING)
	{
		LL_EXTI_DisableIT_0_31(HP_LEFT_EXTI_PIN);
		LL_EXTI_ClearFlag_0_31(HP_LEFT_EXTI_PIN);
		HAL_TIM_Base_Stop(&HP_TIMER);
		HP_DIFFERENCE_VALUE = __HAL_TIM_GET_COUNTER(&HP_TIMER);
		__HAL_TIM_SET_COUNTER(&HP_TIMER, 0);
		HP_STATUS = HP_STOPPED;
	}
}

void RIGHT_EXTI_Arrived()
{
	HP_RIGHT_COME_COUNTER++;
	if(HP_STATUS == HP_IDLE)
	{
		LL_EXTI_DisableIT_0_31(HP_RIGHT_EXTI_PIN);
		LL_EXTI_ClearFlag_0_31(HP_RIGHT_EXTI_PIN);
		HAL_TIM_Base_Start_IT(&HP_TIMER);
		HP_FIRST_COME = RIGHT_SIGNAL;
		HP_STATUS = HP_PENDING;

	}
	else if(HP_STATUS == HP_PENDING)
	{
		LL_EXTI_DisableIT_0_31(HP_RIGHT_EXTI_PIN);
		LL_EXTI_ClearFlag_0_31(HP_RIGHT_EXTI_PIN);
		HAL_TIM_Base_Stop(&HP_TIMER);
		HP_DIFFERENCE_VALUE = __HAL_TIM_GET_COUNTER(&HP_TIMER);
		__HAL_TIM_SET_COUNTER(&HP_TIMER, 0);
		HP_STATUS = HP_STOPPED;
	}
}

typedef union
{
	struct{
		uint8_t header;
		uint8_t cmd;
		int16_t imu;
		int16_t sound;
		uint8_t checksum;
	};
	uint8_t protocol[7];
}Protocol;

void sendDatum(uint8_t cmd)
{
	static Protocol sendProtocol;
	sendProtocol.header = 0x55;
	sendProtocol.cmd = cmd;
	sendProtocol.imu = AEM_IMU.BNO055_Addr->ProcessedEuler.Yaw_Axis;
	sendProtocol.sound = AEM_AudioSignalControl.HP_LastSignalDetails.HP_SignalSourceAngle;

	uint16_t checksumZartZurt = 0;
	for(uint8_t i = 0; i < 6; i++)
	{
		checksumZartZurt += sendProtocol.protocol[i];
	}
	sendProtocol.checksum = checksumZartZurt % 0xFF;
	HAL_UART_Transmit_IT(&huart1, sendProtocol.protocol, 7);
}

void AudioSignalProcess()
{
	if(HP_STATUS == HP_IDLE)
	{
		if(HP_OLD_STATUS != HP_IDLE)
		{
			LL_EXTI_EnableIT_0_31(HP_LEFT_EXTI_PIN);
			LL_EXTI_EnableIT_0_31(HP_RIGHT_EXTI_PIN);
			HP_LEFT_COME_COUNTER = 0;
			HP_RIGHT_COME_COUNTER = 0;

		}
		sendDatum(0);
		HP_OLD_STATUS = HP_STATUS;
	}
	else if(HP_STATUS == HP_PENDING)
	{

		HP_OLD_STATUS = HP_STATUS;
	}
	else if(HP_STATUS == HP_STOPPED)
	{

		if(HP_DIFFERENCE_VALUE > HP_CONSTANT_MAX)
		{
			HP_SIGNAL_ANGLE = -1;
			HP_STATUS = HP_ERROR;
			sendDatum(0);
		}
		else if(AEM_AudioSignalControl.HP_LastSignalDetails.HP_FirstSignalCome == LEFT_SIGNAL)
		{
			if(HP_DIFFERENCE_VALUE > HP_DYNAMIC_LEFT_MAX)
			{
				HP_DYNAMIC_LEFT_MAX = HP_DIFFERENCE_VALUE;
			}
			HP_SIGNAL_ANGLE = 9000 / (HP_DYNAMIC_LEFT_MAX + 1);
			HP_SIGNAL_ANGLE = HP_SIGNAL_ANGLE * HP_DIFFERENCE_VALUE;
			HP_SIGNAL_ANGLE /= 100;
			sendDatum(HP_FIRST_COME);
		}
		else if(AEM_AudioSignalControl.HP_LastSignalDetails.HP_FirstSignalCome == RIGHT_SIGNAL)
		{
			if(HP_DIFFERENCE_VALUE > HP_DYNAMIC_RIGHT_MAX)
			{
				HP_DYNAMIC_RIGHT_MAX = HP_DIFFERENCE_VALUE;
			}
			HP_SIGNAL_ANGLE = 9000 / (HP_DYNAMIC_RIGHT_MAX + 1);
			HP_SIGNAL_ANGLE = HP_SIGNAL_ANGLE * HP_DIFFERENCE_VALUE;
			HP_SIGNAL_ANGLE /= 100;
			sendDatum(HP_FIRST_COME);
		}
		HP_DELAY_COUNTER = 1000;
		HP_OLD_STATUS = HP_STATUS;
		HP_STATUS = HP_DELAY;
	}
	else if(HP_STATUS == HP_DELAY)
	{
		HP_OLD_STATUS = HP_STATUS;
		if(HP_DELAY_COUNTER == 0)
		{
			HP_STATUS = HP_IDLE;
		}
	}
	else if(HP_STATUS == HP_ERROR)
	{
		LL_EXTI_DisableIT_0_31(HP_RIGHT_EXTI_PIN);
		LL_EXTI_DisableIT_0_31(HP_LEFT_EXTI_PIN);
		HAL_TIM_Base_Stop(&HP_TIMER);
		HP_OLD_STATUS = HP_STATUS;
		if(HP_DELAY_COUNTER == 0)
		{
			HP_STATUS = HP_IDLE;
		}
		sendDatum(0);
	}
	else
	{
		HP_OLD_STATUS = HP_STATUS;
		HP_STATUS = HP_IDLE;
	}
}
