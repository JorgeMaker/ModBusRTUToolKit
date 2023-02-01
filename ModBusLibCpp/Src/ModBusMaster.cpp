/*******************************
 *      ModBusMaster.cpp
 *
 *  Created on: 1 feb. 2023
 *   Author: JorgeMaker
*******************************/
#include "../Inc/ModBusMaster.h"
#include "string.h"

ModbusMaster::ModbusMaster(UART_HandleTypeDef *serailPort,
		GPIO_TypeDef *enablePort, uint16_t pinNumber) :
		ModBusDevice(serailPort, enablePort, pinNumber) {

}

ModBusResult ModbusMaster::sendReadCoilsRequest(uint8_t slaveID,
		uint16_t startingCoil, uint16_t numOfCoils) {

	txFrame[0] = slaveID;  		// Slave address
	txFrame[1] = READ_COILs; 	// 0x01 Function code for Read Coils

	txFrame[2] = (startingCoil >> 8) & 0xff;
	txFrame[3] = startingCoil & 0xff;

	txFrame[4] = (numOfCoils >> 8) & 0xff;
	txFrame[5] = numOfCoils & 0xff;

	sendModBusRequest(6);
	return MODBUS_OK;

}

ModBusResult ModbusMaster::sendReadDiscInputsRequest(uint8_t slaveID,
		uint16_t startingCoil, uint16_t offset) {

	txFrame[0] = slaveID;  					// Slave address
	txFrame[1] = READ_DISC_INPUTs; 	// 0x02 Function code for Read Coils

	txFrame[2] = (startingCoil >> 8) & 0xff;
	txFrame[3] = startingCoil & 0xff;

	txFrame[4] = (offset >> 8) & 0xff;
	txFrame[5] = offset & 0xff;

	sendModBusRequest(6);
	return MODBUS_OK;

}

ModBusResult ModbusMaster::sendReadHoldingRegsRequest(uint8_t slaveID,
		uint16_t startingReg, uint16_t numOfregs) {

	txFrame[0] = slaveID;  // Slave address
	txFrame[1] = READ_HOLDING_REGs; // 0x03 Function code for Read Holding Registers

	//The Register address will be startingReg  at address 40001
	txFrame[2] = (startingReg >> 8) & 0xff;
	txFrame[3] = startingReg & 0xff;

	// numOfregs of registers to read will be read
	txFrame[4] = (numOfregs >> 8) & 0xff;
	txFrame[5] = numOfregs & 0xff;

	sendModBusRequest(6);
	return MODBUS_OK;

}

ModBusResult ModbusMaster::sendReadInputRegsRequest(uint8_t slaveID,
		uint16_t startingReg, uint16_t offset) {

	txFrame[0] = slaveID;  // Slave address
	txFrame[1] = READ_INPUT_REGs; // 0x04 Function code for Read Input Registers

	txFrame[2] = (startingReg >> 8) & 0xff;
	txFrame[3] = startingReg & 0xff;

	//numOfregs of input regs to read
	txFrame[4] = (offset >> 8) & 0xff;
	txFrame[5] = offset & 0xff;

	sendModBusRequest(6);
	return MODBUS_OK;
}

ModBusResult ModbusMaster::sendWriteSingleCoilRequest(uint8_t slaveID,
		uint16_t coilNum, uint16_t value) {

	txFrame[0] = slaveID;  			// Slave address
	txFrame[1] = WRITE_SINGLE_COIL; // 0x05 Function code for Read Coils

	txFrame[2] = ((coilNum - 1) >> 8) & 0xff;
	txFrame[3] = (coilNum - 1) & 0xff;

	if (value == 1) {
		txFrame[4] = 0XFF;
		txFrame[5] = 0x00;
	} else if (value == 0) {
		txFrame[4] = 0x00;
		txFrame[5] = 0x00;
	}

	sendModBusRequest(6);
	return MODBUS_OK;

}

ModBusResult ModbusMaster::sendWriteSingleHoldingReg(uint8_t slaveID,
		uint16_t regNum, uint16_t value) {

	txFrame[0] = slaveID;  			// Slave address
	txFrame[1] = WRITE_SINGLE_REG; // 0x06 Function code for Write single Register.

	// TODO Revisar lo del valo del pirmer registro si es 1 o es 0 ???
	txFrame[2] = ((regNum - 1) >> 8) & 0xff; //Address = 40.001 + reglNum
	txFrame[3] = (regNum - 1) & 0xff;

	txFrame[4] = (value >> 8) & 0xff;
	txFrame[5] = value & 0xff;

	sendModBusRequest(6);
	return MODBUS_OK;
}

ModBusResult ModbusMaster::sendWriteMultipleHoldingReg(uint8_t slaveID,
		uint16_t startingReg, uint8_t regNum, uint16_t *values) {

	txFrame[0] = slaveID;  				// Slave address
	txFrame[1] = WRITE_HOLDING_REGs; // 0x10 // 16 Function code for Write multiples Registers

	txFrame[2] = (startingReg >> 8) & 0xff;
	txFrame[3] = startingReg & 0xff;

	txFrame[4] = 0x00;
	txFrame[5] = regNum;

	txFrame[6] = regNum * 2;  // Byte Count = 2 bytes for register

	for (uint8_t regCounter = 0; regCounter < regNum; regCounter++) {

		txFrame[7 + regCounter * 2] = (values[regCounter] >> 8) & 0xff;
		txFrame[8 + regCounter * 2] = values[regCounter] & 0xff;
	}

	sendModBusRequest(7 + regNum * 2);
	return MODBUS_OK;
}

ModBusResult ModbusMaster::sendWriteMultipleCoils(uint8_t slaveID,
		uint16_t startingCoil, uint16_t numOfCoils, uint8_t *coilValues) {

	txFrame[0] = slaveID;  			// Slave address
	txFrame[1] = WRITE_MULT_COILs; // 0x0F // 15 Function code for Write multiples coils

	txFrame[2] = (startingCoil >> 8) & 0xff;
	txFrame[3] = startingCoil & 0xff;

	txFrame[4] = (numOfCoils >> 8) & 0xff;
	txFrame[5] = numOfCoils & 0xff;

	txFrame[6] = encodeCoils(coilValues, numOfCoils, txFrame + 7);
	uint8_t payloadLen = 7 + txFrame[6];

	sendModBusRequest(payloadLen);
	return MODBUS_OK;
}

ModBusReadMutilpleResult ModbusMaster::handleReadHoldingRegsResult() {

	ModBusReadMutilpleResult result;

	uint8_t *frame = getRxBufferReferece();
	result.slave_ID = frame[0];
	result.opCode = frame[1];
	result.byteCounter = frame[2];
	memcpy(result.value, frame + 3, (size_t) frame[2] * 2);

	return result;

}

ModBusReadMutilpleResult ModbusMaster::handleReadInputRegsResult() {

	ModBusReadMutilpleResult result;

	uint8_t *frame = getRxBufferReferece();
	result.slave_ID = frame[0];
	result.opCode = frame[1];
	result.byteCounter = frame[2];
	memcpy(result.value, frame + 3, (size_t) frame[2] * 2);

	return result;

}

ModBusReadMutilpleResult ModbusMaster::handleReadDiscInputsResult() {
	ModBusReadMutilpleResult result;

	uint8_t *frame = getRxBufferReferece();
	result.slave_ID = frame[0];
	result.opCode = frame[1];
	result.byteCounter = frame[2];
	memcpy(result.value, frame + 3, (size_t) frame[2] * 2);

	return result;

}

ModBusReadMutilpleResult ModbusMaster::handleReadCoilsResult() {

	ModBusReadMutilpleResult result;

	uint8_t *frame = getRxBufferReferece();
	result.slave_ID = frame[0];
	result.opCode = frame[1];
	result.byteCounter = frame[2];
	memcpy(result.value, frame + 3, (size_t) frame[2] * 2);

	return result;

}

ModBusWriteSingleResult ModbusMaster::handleWriteSingleCoilResult() {

	ModBusWriteSingleResult result;
	uint8_t *frame = getRxBufferReferece();

	result.slave_ID = frame[0];
	result.opCode = frame[1];

	result.adress = (frame[2] << 8) | frame[3];
	result.value = (frame[4] << 8) | frame[5];

	return result;

}

ModBusWriteSingleResult ModbusMaster::handleWriteSingleHoldingRegResult() {

	ModBusWriteSingleResult result;
	uint8_t *frame = getRxBufferReferece();

	result.slave_ID = frame[0];
	result.opCode = frame[1];

	result.adress = (frame[2] << 8) | frame[3];
	result.value = (frame[4] << 8) | frame[5];

	return result;

}

ModBusWriteMultipleResult ModbusMaster::handleWriteMultipleCoilsResult() {

	ModBusWriteMultipleResult result;

	uint8_t *frame = getRxBufferReferece();

	result.slave_ID = frame[0];
	result.opCode = frame[1];

	result.startAddr = (frame[2] << 8) | frame[3];
	result.endAddr = (frame[4] << 8) | frame[5];

	return result;

}

ModBusWriteMultipleResult ModbusMaster::handleWriteMultipleHoldingRegResult() {

	ModBusWriteMultipleResult result;

	uint8_t *frame = getRxBufferReferece();

	result.slave_ID = frame[0];
	result.opCode = frame[1];

	result.startAddr = (frame[2] << 8) | frame[3];
	result.endAddr = (frame[4] << 8) | frame[5];

	return result;

}
