/*
 * FK_Uart.c
 *
 *  Created on: Dec 9, 2021
 *      Author: Ali Eren Mercan
 */


#include "FK_Uart.h"
#include "string.h"


_FK_UART FK_UART = {0};


void UartInit(){

	UART_TIMEOUT_COUNTER = 0;
	UART_RX_DATA_READY_FLAG = 0;
	UART_RX_IT_STEP = RHEADER;
	UART_RX_STATUS = UART_ERROR;
	UART_RX_HEADER = RX_HEADER;

	UART_TX_BUF[0] = (TX_HEADER & 0xFF);
	UART_TX_BUF[1] = ((TX_HEADER >> 8) & 0xFF);

	
	HAL_UART_RegisterCallback(&UART_PORT, HAL_UART_RX_COMPLETE_CB_ID, UartByteInterrupt);
	HAL_UART_Receive_IT(&UART_PORT, &UART_RX_BYTE, 1);
}

void UartByteInterrupt(){
	UartRecieveByte(UART_RX_BYTE);
	HAL_UART_Receive_IT(&UART_PORT, &UART_RX_BYTE, 1);
}

void UartRecieveByte(uint8_t RxByte){
	static uint8_t i = 0;

		if(UART_RX_IT_STEP == RHEADER){

			if(((UART_RX_HEADER >> (i * 8)) & 0xFF) == RxByte){
				i++;
				if(i == RX_HEADER_SIZE){
					i = 0;
					UART_RX_IT_STEP = RDATA;
				}
			}
			else{
				i = 0;
			}

		}
		else if (UART_RX_IT_STEP == RDATA){

			UART_RX_DYNAMIC_BUF[i++] = RxByte;
			if(i == UART_RX_DATA_W_CRC_SIZE){
				memcpy(UART_RX_RECIEVED_BUF, UART_RX_DYNAMIC_BUF, UART_RX_SIZE);
				UART_RX_DATA_READY_FLAG = 1;
				UART_RX_IT_STEP = RHEADER;
				i = 0;
			}
		}
}

void CommunicationProcess(){
	GetUartData();
	CommControl();
}

void CommControl(){
	if(++UART_TIMEOUT_COUNTER > UART_TIMEOUT_LIMIT){
		UART_TIMEOUT_COUNTER = UART_TIMEOUT_LIMIT;
		UART_RX_STATUS = UART_ERROR;
		UartInit();
	}
	else{
	}
}

void GetUartData(){
	if(UART_RX_DATA_READY_FLAG){
		memcpy(UART_RX_CORRECT_BUF, UART_RX_RECIEVED_BUF, UART_RX_DATA_W_CRC_SIZE);
		UART_RX_CRC_VALUE = CRC32(UART_RX_CORRECT_BUF, UART_RX_DATA_W_CRC_SIZE);
		if(UART_RX_CRC_VALUE == 0x00000000){
			memcpy(UART_RX_TEMP_BUF, UART_RX_CORRECT_BUF, UART_RX_DATA_SIZE);
			UART_TIMEOUT_COUNTER = 0;
			UART_RX_STATUS = UART_OK;
		}
		else{
		}
		UART_RX_DATA_READY_FLAG = 0;
	}
}

/* ----------------------------- TRANSMIT-------------------------------------*/
void UartTransmitData(){
	memcpy(UART_TX_BUF + TX_HEADER_SIZE, UART_TX_IN_BUFF_ADDR, UART_TX_DATA_SIZE);
	UART_TX_CRC_VALUE = CRC32(UART_TX_BUF + TX_HEADER_SIZE,  UART_TX_DATA_SIZE);
	for(uint8_t i = 0; i < CRC_SIZE; i++){
		UART_TX_BUF[UART_TX_SIZE - (i + 1)] = ((UART_TX_CRC_VALUE >> (i << 3)) & 0xFF);
	}
	HAL_UART_Transmit_IT(&UART_PORT, UART_TX_BUF, UART_TX_SIZE);
}


/* -------------------------------- Checksum Calculation -----------------------*/
uint32_t CRC32(uint8_t *message, size_t l) {
	uint32_t crc, msb;
	crc = 0xFFFFFFFF;

	for (size_t i = 0; i < l; i++) {
		// xor next byte to upper bits of crc
		crc ^= (((uint32_t) message[i]) << 24);
		for (uint8_t j = 0; j < 8; j++) {    // Do eight times.
			msb = crc >> 31;
			crc <<= 1;
			crc ^= (0 - msb) & 0x04C11DB7;
		}
	}
	return crc;
}
