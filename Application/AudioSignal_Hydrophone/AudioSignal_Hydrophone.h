/*
 * AudioSignal_Hyrophone.h
 */

#ifndef AUDIOSIGNAL_HYDROPHONE_H_
#define AUDIOSIGNAL_HYDROPHONE_H_


#include "main.h"
#include "stm32f1xx_ll_exti.h"


extern TIM_HandleTypeDef 	htim2;
#define HP_TIMER			htim2

#define HP_LEFT_EXTI_PIN	GPIO_PIN_0
#define HP_RIGHT_EXTI_PIN	GPIO_PIN_2



typedef enum
{
	HP_IDLE 		= 0,
	HP_PENDING 		= 1,
	HP_STOPPED 		= 2,
	HP_DELAY		= 3,
	HP_ERROR		= 4

}HP_Status_t;

typedef enum
{
	NULL_SIGNAL		= 0,
	LEFT_SIGNAL		= 1,
	RIGHT_SIGNAL	= 2

}HP_FirstSignalCome_t;

typedef struct
{
	HP_FirstSignalCome_t HP_FirstSignalCome;
	uint32_t HP_ArrivedDifferenceValue;
	uint32_t HP_LeftComeSignalCounter;
	uint32_t HP_RightComeSignalCounter;
	int16_t HP_SignalSourceAngle;

}HP_LastSignalDetails_t;



typedef struct
{
	HP_Status_t Status;
	HP_Status_t oStatus;
	uint32_t HP_DelayCounter;
	HP_LastSignalDetails_t HP_LastSignalDetails;
	uint16_t HP_DynamicLeftMax;
	uint16_t HP_DynamicRightMax;
}AEM_AudioSignalControl_t;
extern AEM_AudioSignalControl_t AEM_AudioSignalControl;

#define HP_STATUS				AEM_AudioSignalControl.Status
#define HP_OLD_STATUS			AEM_AudioSignalControl.oStatus
#define HP_DELAY_COUNTER		AEM_AudioSignalControl.HP_DelayCounter
#define HP_FIRST_COME			AEM_AudioSignalControl.HP_LastSignalDetails.HP_FirstSignalCome
#define HP_DIFFERENCE_VALUE		AEM_AudioSignalControl.HP_LastSignalDetails.HP_ArrivedDifferenceValue
#define HP_LEFT_COME_COUNTER	AEM_AudioSignalControl.HP_LastSignalDetails.HP_LeftComeSignalCounter
#define HP_RIGHT_COME_COUNTER	AEM_AudioSignalControl.HP_LastSignalDetails.HP_RightComeSignalCounter
#define HP_SIGNAL_ANGLE			AEM_AudioSignalControl.HP_LastSignalDetails.HP_SignalSourceAngle

#define HP_CONSTANT_MAX			350
#define HP_DYNAMIC_LEFT_MAX		AEM_AudioSignalControl.HP_DynamicLeftMax
#define HP_DYNAMIC_RIGHT_MAX	AEM_AudioSignalControl.HP_DynamicRightMax
void AudioSignalControlInit();
void SignalPendingTimeoutCallback();
void LEFT_EXTI_Arrived();
void RIGHT_EXTI_Arrived();
void AudioSignalProcess();

#endif /* AUDIOSIGNAL_HYDROPHONE_H_ */
