/*
 * Application.c
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */

#include "AudioSignal_Hydrophone.h"
AEM_AudioSignalControl_t AEM_AudioSignalControl;

void AudioSignalControlInit()
{
	AEM_AudioSignalControl.LeftArrived = 0;
	AEM_AudioSignalControl.RightArrived = 0;
	AEM_AudioSignalControl.timerCnt = 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0 && !AEM_AudioSignalControl.LeftArrived)
	{
		AEM_AudioSignalControl.LeftArrived = 1;

		  //HAL_TIM_RegisterCallback(&htim4, HAL_TIM_PERIOD_ELAPSED_CB_ID, SysTimerUpdate);
		  HAL_TIM_Base_Start(&htim2);
	}
	else if(GPIO_Pin == GPIO_PIN_2 && !AEM_AudioSignalControl.RightArrived)
	{
		AEM_AudioSignalControl.RightArrived = 0;
		AEM_AudioSignalControl.LeftArrived = 0;
		HAL_TIM_Base_Stop(&htim2);
		AEM_AudioSignalControl.timerCnt = __HAL_TIM_GET_COUNTER(&htim2);
	}
}
