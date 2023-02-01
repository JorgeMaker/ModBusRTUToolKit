/*******************************
 *      modbusMaster.h
 *
 *  Created on: 1 feb. 2023
 *   Author: JorgeMaker
*******************************/

#ifndef INC_MODBUSMASTER_H_
#define INC_MODBUSMASTER_H_

#include "main.h"

//void sendModBusRequest(UART_HandleTypeDef huart, uint8_t *frame, uint8_t len);
void sendReadHoldingRegsRequest(UART_HandleTypeDef huart,uint8_t slaveID, uint16_t startingReg,uint16_t numOfregs);
void sendReadInputRegsRequest(UART_HandleTypeDef huart,uint8_t slaveID, uint16_t startingReg,uint16_t offset);
void sendReadDiscInputsRequest(UART_HandleTypeDef huart,uint8_t slaveID, uint16_t startingCoil,uint16_t offset);
void sendReadCoilsRequest(UART_HandleTypeDef huart,uint8_t slaveID, uint16_t startingCoil,uint16_t numOfCoils);
void sendWriteSingleCoilRequest(UART_HandleTypeDef huart,uint8_t slaveID, uint16_t coilNum,uint16_t value);
void sendWriteSingleHoldingReg(UART_HandleTypeDef huart,uint8_t slaveID, uint16_t regNum, uint16_t value);
void sendWriteMultipleHoldingReg(UART_HandleTypeDef huart,uint8_t slaveID,uint16_t startingReg, uint8_t regNum,uint16_t* values);
void sendWriteMultipleCoils(UART_HandleTypeDef huart,uint8_t slaveID, uint16_t startingCoil, uint16_t numOfCoils,uint8_t* coilValues);



#endif /* INC_MODBUSMASTER_H_ */
