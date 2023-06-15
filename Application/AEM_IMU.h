/*
 * AEM_IMU.h
 *
 *  Created on: Dec 30, 2021
 *      Author: Ali Eren Mercan
 */

#ifndef AEM_IMU_H_
#define AEM_IMU_H_

#include <stdint.h>
#include "BNO055.h"


#define			IMU_TASK_HZ			50
#define 		IMU_I2C_TIMEOUT		(IMU_TASK_HZ >> 1)

extern I2C_HandleTypeDef hi2c2;
#define			IMU_I2C_PORT	hi2c2

#define IMU_RESET_PORT				GPIOB
#define IMU_RESET_PIN				GPIO_PIN_8

#define I2C_SHIFTER_EN_PORT			GPIOC
#define I2C_SHIFTER_EN_PIN			GPIO_PIN_14




typedef enum{
	IMU_I2C_IDLE	=		0,
	IMU_I2C_PENDING =		1
}_IMU_I2C_Status;


typedef enum{
	IMU_INIT 	= 		0,
	IMU_READ 	= 		1,
	IMU_DELAY 	= 		2
}_IMU_Status;

typedef enum{
	IMU_NOT_ONCE_READED = 		0,
	IMU_ONCE_READED 	= 		1
}_IMU_OnceRead;

typedef enum{
	IMU_LOW_nRESET			=		0,
	IMU_HIGH_nRESET			=		1,
	IMU_GET_CHIP_ID			=		2,
	IMU_CONTROL_CHIP_ID		=		3,
	IMU_SET_CALIB_OFFSET	=		4,
	IMU_USE_EXTERNAL_OSC	=		5,
	IMU_SET_OP_MOD_NDOF		=		6,
	IMU_END_INIT			=		7
}_Init_Step;


typedef struct{
	_IMU_I2C_Status			IMU_I2C_Status;
	_IMU_Status 			IMU_Status;
	_IMU_Status				oIMU_Status;
	_IMU_OnceRead			IMU_OnceRead;
	_Init_Step				Init_Step;
	_BNO055					*BNO055_Addr;



	uint32_t				PendingTimeout;
	uint32_t				DelayCounter;
	uint32_t				DelayConstant;
}_AEM_IMU;
extern _AEM_IMU AEM_IMU;


#define		IMU_I2C_STATUS			AEM_IMU.IMU_I2C_Status
#define		IMU_STATUS				AEM_IMU.IMU_Status
#define		OLD_IMU_STATUS			AEM_IMU.oIMU_Status
#define		IMU_ONCE_READ			AEM_IMU.IMU_OnceRead
#define		IMU_INIT_STEP			AEM_IMU.Init_Step
#define		BNO055_TYPEDEF_PTR		AEM_IMU.BNO055_Addr


#define		IMU_PENDING_TIMEOUT			AEM_IMU.PendingTimeout
#define		IMU_DELAY_COUNTER		AEM_IMU.DelayCounter
#define		IMU_DELAY_CONSTANT		AEM_IMU.DelayConstant

#define		IMU_EULER_RAW_DATA				AEM_IMU.BNO055_Addr->RawEuler
#define		IMU_EULER_PROCESSED_DATA		AEM_IMU.BNO055_Addr->ProcessedEuler


void IMU_Process();

void AEM_IMU_Init();
void BNO055_DriverInit();
void SetConstantCalibrationOffsets();

void IMU_WhenI2C_Pending();
void IMU_WhenI2C_Idle();

void IMU_ProcessInit();

void IMU_ProcessRead();
void IMU_RawEulerConvert();

void IMU_ProcessDelay();

void IMU_DelayStart(uint32_t ms);

void IMU_TRANSMIT_IT();
void IMU_RECEIVE_IT();




#endif /* AEM_IMU_H_ */
