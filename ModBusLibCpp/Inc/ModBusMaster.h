/*******************************
 *      modbusMaster.h
 *
 *  Created on: 1 feb. 2023
 *   Author: JorgeMaker
*******************************/

#ifndef INC_MODBUSMASTER_H_
#define INC_MODBUSMASTER_H_

#include "ModBusDevice.h"

/**
 *  Class: ModBusDevice ModbusMaster
 *         Represents a Modbus master device.
 *
 * ModbusMaster is a derived class of ModBusDevice and extends its functionality to perform
 * as a Modbus master device. It has several functions to send requests and handle results of
 * different types of Modbus operations such as reading holding registers, inputs, discrete inputs,
 * coils, writing single coil or holding register, and writing multiple coils or holding registers.
 *
 */

typedef struct{
	uint8_t slave_ID;
	uint8_t opCode;
	uint8_t byteCounter;
	uint8_t value[250];

} ModBusReadMutilpleResult;

typedef struct{
	uint8_t slave_ID;
	uint8_t opCode;
	uint16_t addr;
	uint16_t value;

} ModBusReadSingleResult;

typedef struct{
	uint8_t slave_ID;
	uint8_t opCode;
	uint16_t startAddr;
	uint16_t endAddr;

} ModBusWriteMultipleResult;

typedef struct{
	uint8_t slave_ID;
	uint8_t opCode;
	uint8_t adress;
	uint8_t value;

} ModBusWriteSingleResult;


class ModbusMaster: public ModBusDevice{
	public:
		ModbusMaster(UART_HandleTypeDef* serailPort,GPIO_TypeDef*  enablePort, uint16_t pinNumber);

		ModBusResult sendReadHoldingRegsRequest(uint8_t slaveID, uint16_t startingReg,uint16_t numOfregs);
		ModBusResult sendReadInputRegsRequest(uint8_t slaveID, uint16_t startingReg,uint16_t offset);
		ModBusResult sendReadDiscInputsRequest(uint8_t slaveID, uint16_t startingCoil,uint16_t offset);
		ModBusResult sendReadCoilsRequest(uint8_t slaveID, uint16_t startingCoil,uint16_t numOfCoils);
		ModBusResult sendWriteSingleCoilRequest(uint8_t slaveID, uint16_t coilNum,uint16_t value);
		ModBusResult sendWriteSingleHoldingReg(uint8_t slaveID, uint16_t regNum, uint16_t value);
		ModBusResult sendWriteMultipleHoldingReg(uint8_t slaveID,uint16_t startingReg, uint8_t regNum,uint16_t* values);
		ModBusResult sendWriteMultipleCoils(uint8_t slaveID, uint16_t startingCoil, uint16_t numOfCoils,uint8_t* coilValues);

		static ModBusReadMutilpleResult handleReadHoldingRegsResult();
		static ModBusReadMutilpleResult handleReadInputRegsResult();
		static ModBusReadMutilpleResult handleReadDiscInputsResult();
		static ModBusReadMutilpleResult handleReadCoilsResult();
		static ModBusWriteSingleResult handleWriteSingleCoilResult();
		static ModBusWriteSingleResult handleWriteSingleHoldingRegResult();
		static ModBusWriteMultipleResult handleWriteMultipleCoilsResult();
		static ModBusWriteMultipleResult handleWriteMultipleHoldingRegResult();

};

#endif /* INC_MODBUSMASTER_H_ */
