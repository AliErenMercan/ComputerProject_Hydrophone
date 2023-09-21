/*
 * FK_Uart.h
 *
 *  Created on: Dec 9, 2021
 *      Author: Ali Eren Mercan
 */

#ifndef FK_UART_H_
#define FK_UART_H_

#include "main.h"


/* UART ERROR TIMEOUT */
#define UART_TASK_HZ					10
#define UART_TIMEOUT_LIMIT				UART_TASK_HZ

extern	uint8_t Register[10];
#define UART_TX_IN_BUFF_ADDR			Register

extern UART_HandleTypeDef huart1;
#define UART_PORT						huart1

#define RX_HEADER_SIZE					2
#define RX_HEADER						0x0504
#define	TX_HEADER_SIZE					2
#define	TX_HEADER						0x0504
#define CRC_SIZE						4

/* UART RECEIVE DATA CONFIG */
#define UART_RX_DATA_SIZE				10
#define UART_RX_DATA_W_CRC_SIZE			(UART_RX_DATA_SIZE + CRC_SIZE)
#define UART_RX_SIZE 					(UART_RX_DATA_W_CRC_SIZE + RX_HEADER_SIZE)

/* UART TRANSMIT DATA CONFIG */
#define UART_TX_DATA_SIZE				6
#define UART_TX_DATA_W_CRC_SIZE			(UART_TX_DATA_SIZE + CRC_SIZE)
#define UART_TX_SIZE          			(UART_TX_DATA_W_CRC_SIZE + TX_HEADER_SIZE)


typedef enum{
	RHEADER = 0,
	RDATA,
}_UartRxItStep;

typedef enum{
	UART_ERROR = 0,
	UART_OK
}_UartRxStatus;

typedef struct{
	uint8_t DataReadyFlag;
	_UartRxItStep UartRxItStep;
	_UartRxStatus UartRxStatus;

	uint8_t UartRxByte;
	uint32_t Timeout_Counter;

	uint16_t Header;
	uint32_t CrcValue;

	uint8_t DynamicReceiver[UART_RX_DATA_W_CRC_SIZE];
	uint8_t LastReceivedPacket[UART_RX_DATA_W_CRC_SIZE];
	uint8_t LastCorrectPacket[UART_RX_DATA_W_CRC_SIZE];
	uint8_t Temp[UART_RX_DATA_SIZE];
}_UartRecieve;


typedef struct{
	uint32_t CrcValue;
	uint16_t Header;
	uint8_t *InputBuffer;
	uint8_t TransmissionData[UART_TX_SIZE];
}_UartTransmit;



typedef struct{
	_UartRecieve		UartRecieve;
	_UartTransmit		UartTransmit;
} _FK_UART;
extern _FK_UART FK_UART;

#define UART_RX_IT_STEP				FK_UART.UartRecieve.UartRxItStep
#define UART_RX_STATUS				FK_UART.UartRecieve.UartRxStatus
#define UART_RX_BYTE				FK_UART.UartRecieve.UartRxByte
#define UART_RX_HEADER				FK_UART.UartRecieve.Header
#define UART_RX_DYNAMIC_BUF			FK_UART.UartRecieve.DynamicReceiver
#define UART_RX_RECIEVED_BUF		FK_UART.UartRecieve.LastReceivedPacket
#define UART_RX_CORRECT_BUF			FK_UART.UartRecieve.LastCorrectPacket
#define UART_RX_TEMP_BUF			FK_UART.UartRecieve.Temp
#define UART_RX_CRC_VALUE			FK_UART.UartRecieve.CrcValue
#define UART_TIMEOUT_COUNTER		FK_UART.UartRecieve.Timeout_Counter
#define UART_RX_DATA_READY_FLAG		FK_UART.UartRecieve.DataReadyFlag


#define UART_TX_CRC_VALUE			FK_UART.UartTransmit.CrcValue
#define UART_TX_BUF					FK_UART.UartTransmit.TransmissionData





void UartInit();
void UartByteInterrupt();

/*------------ UART RECEIVE -------------*/
void UartRecieveByte(uint8_t RxByte);
void GetUartData();
int UartDataIsReady();

/*------------ UART TRANSMIT ------------*/
void UartSetTransmitData();
void UartTransmitData();
uint32_t CRC32(uint8_t *message, size_t l);

/* ----------- DEBUG --------------------*/
void CommunicationProcess();
void CommControl();

#endif /* FK_UART_H_ */
