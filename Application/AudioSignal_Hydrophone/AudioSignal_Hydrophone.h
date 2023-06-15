/*
 * AudioSignal_Hyrophone.h
 */

#ifndef AUDIOSIGNAL_HYDROPHONE_H_
#define AUDIOSIGNAL_HYDROPHONE_H_


#include "main.h"

typedef struct
{
	uint8_t LeftArrived;
	uint8_t RightArrived;
	uint32_t timerCnt;
}AEM_AudioSignalControl_t;

extern TIM_HandleTypeDef htim2;




#endif /* AUDIOSIGNAL_HYDROPHONE_H_ */
