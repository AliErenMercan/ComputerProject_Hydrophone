/*
 * BNO055.c
 *
 *  Created on: Dec 30, 2021
 *      Author: Ali Eren Mercan
 */


#include "BNO055.h"

_BNO055 BNO055;

typedef struct{
	uint8_t ChipRegAddr;
	uint8_t *LocalRegAddr;
	uint8_t Length;
}_BNO_I2C_TX_IT_ParameterCarry;
_BNO_I2C_TX_IT_ParameterCarry BNO_I2C_TX_IT_ParameterCarry;

void BNO_I2C_TransmitCallbackForReceiving(){
	HAL_I2C_RegisterCallback(BNO055_I2C_PORT, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, IMU_TX_CALLBACK);
	HAL_I2C_RegisterCallback(BNO055_I2C_PORT, HAL_I2C_MASTER_RX_COMPLETE_CB_ID, IMU_RX_CALLBACK);
	HAL_I2C_Master_Receive_IT(BNO055_I2C_PORT, BNO055_DEV_ADDR<<1, BNO_I2C_TX_IT_ParameterCarry.LocalRegAddr, BNO_I2C_TX_IT_ParameterCarry.Length);

}

void BNO_MemoryRead(uint8_t ChipRegAddr, uint8_t *LocalRegAddr, uint8_t Length){
	HAL_I2C_RegisterCallback(BNO055_I2C_PORT, HAL_I2C_MEM_RX_COMPLETE_CB_ID, IMU_RX_CALLBACK);
	HAL_I2C_Mem_Read_IT(BNO055_I2C_PORT, BNO055_DEV_ADDR<<1, ChipRegAddr, I2C_MEMADD_SIZE_8BIT, LocalRegAddr, Length);

}

void BNO_BusRead(uint8_t ChipRegAddr, uint8_t *LocalRegAddr, uint8_t Length){
	BNO_I2C_TX_IT_ParameterCarry.Length = Length;
	BNO_I2C_TX_IT_ParameterCarry.LocalRegAddr = LocalRegAddr;
	BNO_I2C_TX_IT_ParameterCarry.ChipRegAddr = ChipRegAddr;
	HAL_I2C_RegisterCallback(BNO055_I2C_PORT, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, BNO_I2C_TransmitCallbackForReceiving);
	HAL_I2C_Master_Transmit_IT(BNO055_I2C_PORT, BNO055_DEV_ADDR<<1, &BNO_I2C_TX_IT_ParameterCarry.ChipRegAddr, 1);
}

void BNO_MemoryWrite(uint8_t ChipRegAddr, uint8_t *Data, uint8_t Length){
	static uint8_t BusBuffer[64];
	for(uint8_t i = 0; i < Length; i++){
		BusBuffer[i] = Data[i];
	}
	HAL_I2C_RegisterCallback(BNO055_I2C_PORT, HAL_I2C_MEM_TX_COMPLETE_CB_ID, IMU_TX_CALLBACK);
	HAL_I2C_Mem_Write_IT(BNO055_I2C_PORT, BNO055_DEV_ADDR<<1, ChipRegAddr, I2C_MEMADD_SIZE_8BIT, BusBuffer, Length);
}

void BNO_BusWrite(uint8_t ChipRegAddr, uint8_t *Data, uint8_t Length){
	static uint8_t BusBuffer[64];
	BusBuffer[0] = ChipRegAddr;
	for(uint8_t i = 0; i < Length; i++){
		BusBuffer[i + 1] = Data[i];
	}
	HAL_I2C_RegisterCallback(BNO055_I2C_PORT, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, IMU_TX_CALLBACK);
	HAL_I2C_Master_Transmit_IT(BNO055_I2C_PORT, BNO055_DEV_ADDR<<1, BusBuffer, Length + 1);
}

void BNO_AbortI2C_IT(){
	HAL_I2C_Master_Abort_IT(BNO055_I2C_PORT, BNO055_DEV_ADDR<<1);
	HAL_I2C_DeInit(BNO055_I2C_PORT);
	HAL_I2C_Init(BNO055_I2C_PORT);
}



void BNO_TypedefInit(_BNO055 **BNO055_AddrOnTopLayer){
	*BNO055_AddrOnTopLayer = &BNO055;
	IMU_TX_CALLBACK = NULL;
	BNO055_I2C_PORT = NULL;
	BNO055_ID = BNO055_CHIP_ID;
	BNO055_DEV_ADDR = BNO055_CHIP_I2C_ADDR;
}

void BNO_SetOpMode(_OperationMode OprMode){
	uint8_t OprModeAsAddr = OprMode;
	BNO_MemoryWrite(OPR_MODE_ADDR, &OprModeAsAddr, 1);
}

void BNO_GetCalibrationOffsets(){
	BNO_MemoryRead(BNO055_OFFSET_REG_BEGIN, (uint8_t*)&BNO055_LOCAL_CAL_OFF, 22);
}

void BNO_SetCalibrationOffsets(){
	BNO_MemoryWrite(BNO055_OFFSET_REG_BEGIN, (uint8_t*)&BNO055_LOCAL_CAL_OFF, 22);
}

void BNO_GetChipID(){
	BNO_MemoryRead(BNO055_CHIP_ID_ADDR, (uint8_t*)&BNO055_ID, 1);
}

void BNO_GetEuler(){
	BNO_MemoryRead(BNO055_EULER_H_LSB_ADDR, (uint8_t*)&BNO055_RAW_EULER.Yaw_Axis, 6);
}

void BNO_UseExternalOSC(){
	uint8_t RegValAsAddr = EXT_OSC_ENABLE;
	BNO_MemoryWrite(SYS_TRIGGER_ADDR, &RegValAsAddr, 1);
}
