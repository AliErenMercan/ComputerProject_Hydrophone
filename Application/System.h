/*
 * System.h
 *
 *  Created on: Nov 6, 2021
 *      Author: cayn7
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "main.h"
#define SYSTEM_HZ 			1000
#define CAL(x)				((SYSTEM_HZ/(x)) - 1)

enum{
	IO_TASK_RATE = CAL(10),
	BNO055_TASK_RATE = CAL(50)
};

typedef struct {
	uint32_t IO;
	uint32_t BNO055;
}_Tasks;
extern _Tasks Tasks;



void SysTimerUpdate();

#endif /* SYSTEM_H_ */
