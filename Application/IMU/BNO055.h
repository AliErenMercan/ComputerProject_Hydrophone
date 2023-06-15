/*
 * BNO055.h
 *
 *  Created on: Dec 30, 2021
 *      Author: Ali Eren Mercan
 */

#ifndef INC_BNO055_H_
#define INC_BNO055_H_
#include "main.h"



/*-------------------------------------------------------------------------------*/
/*-----------------------------DEFINES OF BNO055---------------------------------*/
/*-------------------------------------------------------------------------------*/

#define		BNO055_CHIP_I2C_ADDR						(0x28)

#define 	BNO055_CHIP_ID_ADDR                       	(0x00)
#define 	BNO055_CHIP_ID                       		(0xA0)

//Division Factors
#define 	BNO055_ACCEL_DIV_MSQ                       	(100.0)
#define 	BNO055_MAG_DIV_UT                          	(16.0)
#define 	BNO055_GYRO_DIV_DPS                        	(16.0)
#define 	BNO055_GRAVITY_DIV_MSQ                     	(100.0)
#define 	BNO055_LINEAR_ACCEL_DIV_MSQ                	(100.0)
#define 	BNO055_EULER_DIV_DEG                       	(16.0)
#define 	BNO055_EULER_SHIFT                       	4



//System Trigger Register
#define SYS_TRIGGER_ADDR							(0X3F)

#define EXT_OSC_ENABLE    						    (0X80)
#define RST_SYS_ENABLE    						    (0X20)
//System Trigger End

//Operation Mode Register
#define OPR_MODE_ADDR								(0X3D)

#define EXT_OSC_ENABLE    						    (0X80)
#define RST_SYS_ENABLE    						    (0X20)


/* Data Register*/
#define BNO055_ACCEL_DATA_X_LSB_ADDR        (0X08)
#define BNO055_ACCEL_DATA_X_MSB_ADDR        (0X09)
#define BNO055_ACCEL_DATA_Y_LSB_ADDR        (0X0A)
#define BNO055_ACCEL_DATA_Y_MSB_ADDR        (0X0B)
#define BNO055_ACCEL_DATA_Z_LSB_ADDR        (0X0C)
#define BNO055_ACCEL_DATA_Z_MSB_ADDR        (0X0D)
#define BNO055_MAG_DATA_X_LSB_ADDR          (0X0E)
#define BNO055_MAG_DATA_X_MSB_ADDR          (0X0F)
#define BNO055_MAG_DATA_Y_LSB_ADDR          (0X10)
#define BNO055_MAG_DATA_Y_MSB_ADDR          (0X11)
#define BNO055_MAG_DATA_Z_LSB_ADDR          (0X12)
#define BNO055_MAG_DATA_Z_MSB_ADDR          (0X13)
#define BNO055_GYRO_DATA_X_LSB_ADDR         (0X14)
#define BNO055_GYRO_DATA_X_MSB_ADDR         (0X15)
#define BNO055_GYRO_DATA_Y_LSB_ADDR         (0X16)
#define BNO055_GYRO_DATA_Y_MSB_ADDR         (0X17)
#define BNO055_GYRO_DATA_Z_LSB_ADDR         (0X18)
#define BNO055_GYRO_DATA_Z_MSB_ADDR         (0X19)
#define BNO055_EULER_H_LSB_ADDR             (0X1A)
#define BNO055_EULER_H_MSB_ADDR             (0X1B)
#define BNO055_EULER_R_LSB_ADDR             (0X1C)
#define BNO055_EULER_R_MSB_ADDR             (0X1D)
#define BNO055_EULER_P_LSB_ADDR             (0X1E)
#define BNO055_EULER_P_MSB_ADDR             (0X1F)
#define BNO055_QUATERNION_DATA_W_LSB_ADDR   (0X20)
#define BNO055_QUATERNION_DATA_W_MSB_ADDR   (0X21)
#define BNO055_QUATERNION_DATA_X_LSB_ADDR   (0X22)
#define BNO055_QUATERNION_DATA_X_MSB_ADDR   (0X23)
#define BNO055_QUATERNION_DATA_Y_LSB_ADDR   (0X24)
#define BNO055_QUATERNION_DATA_Y_MSB_ADDR   (0X25)
#define BNO055_QUATERNION_DATA_Z_LSB_ADDR   (0X26)
#define BNO055_QUATERNION_DATA_Z_MSB_ADDR   (0X27)
#define BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR (0X28)
#define BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR (0X29)
#define BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR (0X2A)
#define BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR (0X2B)
#define BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR (0X2C)
#define BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR (0X2D)
#define BNO055_GRAVITY_DATA_X_LSB_ADDR      (0X2E)
#define BNO055_GRAVITY_DATA_X_MSB_ADDR      (0X2F)
#define BNO055_GRAVITY_DATA_Y_LSB_ADDR      (0X30)
#define BNO055_GRAVITY_DATA_Y_MSB_ADDR      (0X31)
#define BNO055_GRAVITY_DATA_Z_LSB_ADDR      (0X32)
#define BNO055_GRAVITY_DATA_Z_MSB_ADDR      (0X33)
#define BNO055_TEMP_ADDR                    (0X34)

/*Offset registers*/
#define BNO055_OFFSET_REG_BEGIN				(0X55)
#define BNO055_ACCEL_OFFSET_X_LSB_ADDR      (0X55)
#define BNO055_ACCEL_OFFSET_X_MSB_ADDR      (0X56)
#define BNO055_ACCEL_OFFSET_Y_LSB_ADDR      (0X57)
#define BNO055_ACCEL_OFFSET_Y_MSB_ADDR      (0X58)
#define BNO055_ACCEL_OFFSET_Z_LSB_ADDR      (0X59)
#define BNO055_ACCEL_OFFSET_Z_MSB_ADDR      (0X5A)
#define BNO055_MAG_OFFSET_X_LSB_ADDR        (0X5B)
#define BNO055_MAG_OFFSET_X_MSB_ADDR        (0X5C)
#define BNO055_MAG_OFFSET_Y_LSB_ADDR        (0X5D)
#define BNO055_MAG_OFFSET_Y_MSB_ADDR        (0X5E)
#define BNO055_MAG_OFFSET_Z_LSB_ADDR        (0X5F)
#define BNO055_MAG_OFFSET_Z_MSB_ADDR        (0X60)
#define BNO055_GYRO_OFFSET_X_LSB_ADDR       (0X61)
#define BNO055_GYRO_OFFSET_X_MSB_ADDR       (0X62)
#define BNO055_GYRO_OFFSET_Y_LSB_ADDR       (0X63)
#define BNO055_GYRO_OFFSET_Y_MSB_ADDR       (0X64)
#define BNO055_GYRO_OFFSET_Z_LSB_ADDR       (0X65)
#define BNO055_GYRO_OFFSET_Z_MSB_ADDR       (0X66)
#define BNO055_ACCEL_RADIUS_LSB_ADDR        (0X67)
#define BNO055_ACCEL_RADIUS_MSB_ADDR        (0X68)
#define BNO055_MAG_RADIUS_LSB_ADDR          (0X69)
#define BNO055_MAG_RADIUS_MSB_ADDR          (0X6A)



/*-------------------------------------------------------------------------------*/
/*--------------------------END OF DEFINES OF BNO055-----------------------------*/
/*-------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------*/
/*-------------------------------ENUMS OF BNO055---------------------------------*/
/*-------------------------------------------------------------------------------*/
typedef enum {
	OPERATION_MODE_CONFIG,
	OPERATION_MODE_ACCONLY,
	OPERATION_MODE_MAGONLY,
	OPERATION_MODE_GYRONLY,
	OPERATION_MODE_ACCMAG,
	OPERATION_MODE_ACCGYRO,
	OPERATION_MODE_MAGGYRO,
	OPERATION_MODE_AMG,
	OPERATION_MODE_IMUPLUS,
	OPERATION_MODE_COMPASS,
	OPERATION_MODE_M4G,
	OPERATION_MODE_NDOF_FMC_OFF,
	OPERATION_MODE_NDOF,
}_OperationMode;

/*-------------------------------------------------------------------------------*/
/*----------------------------END OF ENUMS OF BNO055-----------------------------*/
/*-------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------*/
/*------------------------------STRUCTS OF BNO055--------------------------------*/
/*-------------------------------------------------------------------------------*/
typedef struct {
	int16_t X_Axis;
	int16_t Y_Axis;
	int16_t Z_Axis;
}_Vector;


typedef struct {
	int16_t Yaw_Axis;
	int16_t Roll_Axis;
	int16_t Pitch_Axis;
}_EulerVector;


typedef struct {
	uint8_t ReceiveRegister;
	uint8_t AllSensors;
	uint8_t Accelerometer;
	uint8_t Gyroscope;
	uint8_t Magnetometer;
}_BNO_ChipCalibrationOffsets;


typedef struct {
	_Vector AccelerometerOffset;
	_Vector MagnetometerOffset;
	_Vector GyroscopeOffset;
	int16_t AccelerometerRadius;
	int16_t MagnetometerRadius;
}_BNO_LocalCalibrationOffsets;


typedef struct {
	uint8_t ChipID;
	uint8_t DevAddr; /**< i2c device address of bno055 */
	uint8_t SelfTestResult; /*self-test result*/
}_BNO_Chip;

typedef struct {
	I2C_HandleTypeDef* BNO_I2C_Port;
	void (*TopLayerTxCallback)();
	void (*TopLayerRxCallback)();

	_BNO_ChipCalibrationOffsets ChipCalibration;
	_BNO_LocalCalibrationOffsets LocalCalibration;
	_BNO_Chip Chip;
	_Vector Accelerometer_Raw;
	_Vector Magnetometer_Raw;
	_Vector Gyroscope_Raw;
	_Vector Gravity_Raw;
	_Vector LinearAccelerometer_Raw;
	_EulerVector RawEuler;
	_EulerVector ProcessedEuler;
}_BNO055;
extern _BNO055 BNO055;



#define			IMU_TX_CALLBACK			BNO055.TopLayerTxCallback
#define			IMU_RX_CALLBACK			BNO055.TopLayerRxCallback

#define			BNO055_I2C_PORT			BNO055.BNO_I2C_Port
#define			BNO055_DEV_ADDR			BNO055.Chip.DevAddr
#define			BNO055_ID				BNO055.Chip.ChipID
#define			BNO055_CHIP_CAL_OFF		BNO055.ChipCalibration
#define			BNO055_LOCAL_CAL_OFF	BNO055.LocalCalibration


#define			BNO055_RAW_EULER			BNO055.RawEuler
#define			BNO055__PROCESSED_EULER		BNO055.ProcessedEuler

/*-------------------------------------------------------------------------------*/
/*----------------------------END OF STRUCTS OF BNO055---------------------------*/
/*-------------------------------------------------------------------------------*/


void BNO_GetChipID();
void BNO_SetOpMode(_OperationMode OprMode);
void BNO_GetCalibrationOffsets();
void BNO_SetCalibrationOffsets();
void BNO_UseExternalOSC();
void BNO_GetEuler();
void BNO_AbortI2C_IT();
void BNO_TypedefInit(_BNO055 **BNO055_AddrOnTopLayer);


#endif /* INC_BNO055_H_ */
