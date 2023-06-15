/*
 * AEM_IMU.c
 *
 *  Created on: Dec 30, 2021
 *      Author: Ali Eren Mercan
 */

#include "AEM_IMU.h"
#include "InputOutput.h"

_AEM_IMU AEM_IMU;

void AEM_IMU_Init(){
	IMU_I2C_STATUS = IMU_I2C_IDLE;
	IMU_STATUS = IMU_INIT;
	IMU_ONCE_READ = IMU_NOT_ONCE_READED;
	IMU_INIT_STEP = IMU_LOW_nRESET;
	BNO055_DriverInit();
	IMU_PENDING_TIMEOUT = 0;
}

void BNO055_DriverInit(){
	BNO_TypedefInit(&BNO055_TYPEDEF_PTR);
	BNO055_TYPEDEF_PTR->BNO_I2C_Port = &IMU_I2C_PORT;
	BNO055_TYPEDEF_PTR->TopLayerTxCallback = IMU_TRANSMIT_IT;
	BNO055_TYPEDEF_PTR->TopLayerRxCallback = IMU_RECEIVE_IT;
	SetConstantCalibrationOffsets();
}

void SetConstantCalibrationOffsets(){
	BNO055_TYPEDEF_PTR->LocalCalibration.AccelerometerOffset.X_Axis = -26;
	BNO055_TYPEDEF_PTR->LocalCalibration.AccelerometerOffset.Y_Axis = 12;
	BNO055_TYPEDEF_PTR->LocalCalibration.AccelerometerOffset.Z_Axis = -18;
	BNO055_TYPEDEF_PTR->LocalCalibration.GyroscopeOffset.X_Axis = 10;
	BNO055_TYPEDEF_PTR->LocalCalibration.GyroscopeOffset.Y_Axis = -22;
	BNO055_TYPEDEF_PTR->LocalCalibration.GyroscopeOffset.Z_Axis = 2;
	BNO055_TYPEDEF_PTR->LocalCalibration.MagnetometerOffset.X_Axis = 16;
	BNO055_TYPEDEF_PTR->LocalCalibration.MagnetometerOffset.Y_Axis = -292;
	BNO055_TYPEDEF_PTR->LocalCalibration.MagnetometerOffset.Z_Axis = -560;

	BNO055_TYPEDEF_PTR->LocalCalibration.AccelerometerRadius = 1000;
	BNO055_TYPEDEF_PTR->LocalCalibration.MagnetometerRadius = 656;
}

void IMU_Process(){
	if(IMU_I2C_STATUS == IMU_I2C_IDLE){
		IMU_PENDING_TIMEOUT = 0;
		IMU_WhenI2C_Idle();
	}
	else if(IMU_I2C_STATUS == IMU_I2C_PENDING){
		IMU_WhenI2C_Pending();
	}
	else{
		AEM_IMU_Init();
	}
}

void IMU_WhenI2C_Pending(){
	if(++IMU_PENDING_TIMEOUT > IMU_I2C_TIMEOUT){
		BNO_AbortI2C_IT();
		AEM_IMU_Init();
	}
}

void IMU_WhenI2C_Idle(){

	if(IMU_STATUS == IMU_READ){
		IMU_ProcessRead();
	}
	else if(IMU_STATUS == IMU_INIT){
		IMU_ProcessInit();
	}
	else if(IMU_STATUS == IMU_DELAY){
		IMU_ProcessDelay();
	}
	else{
		IMU_STATUS = IMU_INIT;
	}

}


void IMU_ProcessRead(){
	IMU_I2C_STATUS = IMU_I2C_PENDING;
	//BNO_GetCalibrationOffsets();
	BNO_GetEuler();
}

void IMU_RawEulerConvert(){
	IMU_EULER_PROCESSED_DATA.Yaw_Axis = IMU_EULER_RAW_DATA.Yaw_Axis >> BNO055_EULER_SHIFT;
	IMU_EULER_PROCESSED_DATA.Roll_Axis = IMU_EULER_RAW_DATA.Roll_Axis >> BNO055_EULER_SHIFT;
	IMU_EULER_PROCESSED_DATA.Pitch_Axis = IMU_EULER_RAW_DATA.Pitch_Axis >> BNO055_EULER_SHIFT;
}


void IMU_ProcessInit(){
	if(IMU_INIT_STEP == IMU_LOW_nRESET){
						HAL_GPIO_WritePin(IMU_RESET_PORT, IMU_RESET_PIN, 0);
						HAL_GPIO_WritePin(I2C_SHIFTER_EN_PORT, I2C_SHIFTER_EN_PIN, 0);
						IMU_INIT_STEP = IMU_HIGH_nRESET;
	}
	else if(IMU_INIT_STEP == IMU_HIGH_nRESET){
						HAL_GPIO_WritePin(IMU_RESET_PORT, IMU_RESET_PIN, 1);
						HAL_GPIO_WritePin(I2C_SHIFTER_EN_PORT, I2C_SHIFTER_EN_PIN, 1);
						IMU_DelayStart(650);
						IMU_INIT_STEP = IMU_GET_CHIP_ID;
	}
	else if(IMU_INIT_STEP == IMU_GET_CHIP_ID){
						IMU_I2C_STATUS = IMU_I2C_PENDING;
						BNO_GetChipID();
						IMU_INIT_STEP = IMU_CONTROL_CHIP_ID;
	}
	else if(IMU_INIT_STEP == IMU_CONTROL_CHIP_ID){
						if(BNO055_ID != BNO055_CHIP_ID){
							AEM_IMU_Init();
						}
						IMU_INIT_STEP = IMU_SET_CALIB_OFFSET;
	}
	else if(IMU_INIT_STEP == IMU_SET_CALIB_OFFSET){
						IMU_I2C_STATUS = IMU_I2C_PENDING;
						BNO_SetCalibrationOffsets();
						IMU_INIT_STEP = IMU_USE_EXTERNAL_OSC;
	}
	else if(IMU_INIT_STEP == IMU_USE_EXTERNAL_OSC){
						IMU_I2C_STATUS = IMU_I2C_PENDING;
						BNO_UseExternalOSC();
						IMU_DelayStart(650);
						IMU_INIT_STEP = IMU_SET_OP_MOD_NDOF;
	}
	else if(IMU_INIT_STEP == IMU_SET_OP_MOD_NDOF){
						IMU_I2C_STATUS = IMU_I2C_PENDING;
						BNO_SetOpMode(OPERATION_MODE_NDOF);

						IMU_DelayStart(650);
						IMU_INIT_STEP = IMU_END_INIT;
	}
	else if(IMU_INIT_STEP == IMU_END_INIT){
						IMU_STATUS = IMU_READ;
						IMU_INIT_STEP = IMU_LOW_nRESET;
	}
	else{
						AEM_IMU_Init();
	}
}

void IMU_ProcessDelay(){
	if(IMU_DELAY_COUNTER > IMU_DELAY_CONSTANT){
		IMU_STATUS = OLD_IMU_STATUS;
	}
}

void IMU_DelayStart(uint32_t ms){
	IMU_DELAY_COUNTER = 0;
	IMU_DELAY_CONSTANT = ms - 1;
	OLD_IMU_STATUS = IMU_STATUS;
	IMU_STATUS = IMU_DELAY;
}

void IMU_RECEIVE_IT(){
	IMU_I2C_STATUS = IMU_I2C_IDLE;

	if(IMU_STATUS == IMU_READ){
		IMU_RawEulerConvert();
		IMU_ONCE_READ = IMU_ONCE_READED;
	}
}

void IMU_TRANSMIT_IT(){
	IMU_I2C_STATUS = IMU_I2C_IDLE;
}
