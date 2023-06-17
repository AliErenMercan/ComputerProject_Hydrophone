/*
 * Application.c
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */

#include "AudioSignal_Hydrophone.h"
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
		HP_OLD_STATUS = HP_STATUS;
	}
	else if(HP_STATUS == HP_PENDING)
	{

		HP_OLD_STATUS = HP_STATUS;
	}
	else if(HP_STATUS == HP_STOPPED)
	{
		//CALCULATE

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
	}
	else
	{
		HP_OLD_STATUS = HP_STATUS;
		HP_STATUS = HP_IDLE;
	}
}
