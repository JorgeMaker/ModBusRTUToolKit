/*******************************
 *      modbusMaster.c
 *
 *  Created on: 1 feb. 2023
 *   Author: JorgeMaker
*******************************/


#include "modbusDevice.h"
#include "main.h"


const uint8_t MAX_WRITE_COILS = 200;
const uint8_t MAX_WRITE_REGS  = 128;


void sendReadHoldingRegsRequest(UART_HandleTypeDef* huart, uint8_t slaveID, uint16_t startingReg,
		uint16_t numOfregs) {

	uint8_t requestFrame[8];

	requestFrame[0] = slaveID;  // slave address
	requestFrame[1] = READ_HOLDING_REGs; // 0x03 Function code for Read Holding Registers

	//The Register address will be startingReg  at address 40001
	requestFrame[2] = (startingReg >> 8) & 0xff;
	requestFrame[3] = startingReg & 0xff;

	// numOfregs of registers to read will be read
	requestFrame[4] = (numOfregs >> 8) & 0xff;
	requestFrame[5] = numOfregs & 0xff;

    sendModBusRequest(huart, requestFrame,6);

}

void sendReadInputRegsRequest(UART_HandleTypeDef* huart, uint8_t slaveID, uint16_t startingReg,uint16_t numOfregs) {

	uint8_t requestFrame[8];

	requestFrame[0] = slaveID;  // slave address
	requestFrame[1] = READ_INPUT_REGs; // 0x04 Function code for Read Input Registers


	requestFrame[2] = (startingReg >> 8) & 0xff;
	requestFrame[3] = startingReg & 0xff;

	//numOfregs of registers to read will be read
	requestFrame[4] = (numOfregs >> 8) & 0xff;
	requestFrame[5] = numOfregs & 0xff;

	sendModBusRequest(huart, requestFrame,6);
}

void sendReadDiscInputsRequest(UART_HandleTypeDef* huart, uint8_t slaveID, uint16_t startingCoil,
		uint16_t numOfCoils) {

	uint8_t requestFrame[8];

	requestFrame[0] = slaveID;  // slave address
	requestFrame[1] = READ_DISC_INPUTs; // 0x02 Function code for Read Coils

	requestFrame[2] = (startingCoil >> 8) & 0xff;
	requestFrame[3] = startingCoil & 0xff;

	requestFrame[4] = (numOfCoils >> 8) & 0xff;
	requestFrame[5] = numOfCoils & 0xff;

	sendModBusRequest(huart, requestFrame,6);

}
void sendReadCoilsRequest(UART_HandleTypeDef* huart, uint8_t slaveID, uint16_t startingCoil,
		uint16_t numOfCoils) {

	uint8_t requestFrame[8];

	requestFrame[0] = slaveID;  // slave address
	requestFrame[1] = READ_COILs; // 0x01 Function code for Read Coils

	requestFrame[2] = (startingCoil >> 8) & 0xff;
	requestFrame[3] = startingCoil & 0xff;

	requestFrame[4] = (numOfCoils >> 8) & 0xff;
	requestFrame[5] = numOfCoils & 0xff;

	sendModBusRequest(huart, requestFrame,6);

}

void sendWriteSingleCoilRequest(UART_HandleTypeDef* huart, uint8_t slaveID, uint16_t coilNum,
		uint16_t value) {

	uint8_t requestFrame[8];

	requestFrame[0] = slaveID;  // slave address
	requestFrame[1] = WRITE_SINGLE_COIL; // 0x05 Function code for Read Coils

	requestFrame[2] = ((coilNum - 1) >> 8) & 0xff;
	requestFrame[3] = (coilNum - 1) & 0xff;

	if (value == 1) {
		requestFrame[4] = 0xff;
		requestFrame[5] = 0x00;
	} else if (value == 0) {
		requestFrame[4] = 0x00;
		requestFrame[5] = 0x00;
	}


	sendModBusRequest(huart, requestFrame,6);
}

void sendWriteSingleHoldingReg(UART_HandleTypeDef* huart, uint8_t slaveID, uint16_t regNum, uint16_t value) {

	uint8_t requestFrame[8];

	requestFrame[0] = slaveID;  // slave address
	requestFrame[1] = WRITE_SINGLE_REG; // 0x06 Function code for Write single Register.

	// TODO Revisar lo del valo del pirmer registro si es 1 o es 0 ???
	requestFrame[2] = ((regNum - 1) >> 8) & 0xff; //Address = 40.001 + reglNum
	requestFrame[3] = (regNum - 1) & 0xff;

	requestFrame[4] = (value >> 8) & 0xff;
	requestFrame[5] = value & 0xff;

	sendModBusRequest(huart, requestFrame,6);

}

void sendWriteMultipleCoils(UART_HandleTypeDef* huart, uint8_t slaveID, uint16_t startingCoil, uint16_t numOfCoils,uint8_t* coilValues) {


	uint8_t requestFrame[7+MAX_WRITE_COILS];

	requestFrame[0] = slaveID;  // slave address
	requestFrame[1] = WRITE_MULT_COILs; // 0x0F // 15 Function code for Write multiples coils

	requestFrame[2] = (startingCoil >> 8) & 0xff;
	requestFrame[3] = startingCoil & 0xff;

	requestFrame[4] = (numOfCoils >> 8) & 0xff;
	requestFrame[5] = numOfCoils & 0xff;

	requestFrame[6] = encodeCoils(coilValues,numOfCoils,requestFrame+7);

	uint8_t payloadLen=  7+ requestFrame[6];

	sendModBusRequest(huart, requestFrame, payloadLen);

}

uint8_t encodeCoils(uint8_t* coilValues, uint8_t coilsNum, uint8_t* output_array) {

	uint8_t coilsCursor =0;
	uint8_t bytesCounter =0;
    uint8_t dataToEncode[8] = {};

	while(coilsNum > coilsCursor){

		memset(dataToEncode, 0, 8);
		uint8_t chunk = 0;
		if (coilsCursor+8<=coilsNum){
			chunk= 8;
		}
		else {
			chunk= coilsNum-coilsCursor;
		}
		copyFrom_S_to_E_UpTo8Values(coilValues,dataToEncode,coilsCursor,coilsCursor+chunk);
		uint8_t currentOutputByte =  encodeBitsIntoByte(dataToEncode,chunk);
		output_array[bytesCounter] = currentOutputByte;
		bytesCounter++;
		coilsCursor = coilsCursor +8;
	}
	return bytesCounter;
}

uint8_t encodeBitsIntoByte (uint8_t *bitToEncode, uint8_t len){

	uint8_t encodedValue = 0;
    int bitCursor;
    for (bitCursor = 0; (bitCursor < 8) && (bitCursor< len); bitCursor++) {
    	encodedValue |= (bitToEncode[bitCursor] << (7 - bitCursor));
    }
	return encodedValue;
}


void copyFrom_S_to_E_UpTo8Values(uint8_t* origin, uint8_t* destination, uint8_t start,  uint8_t end) {
	size_t copy_size = end-start;
    if (copy_size >8) {
        copy_size = 8;
    }
	memcpy(destination,origin+start,(size_t)copy_size);
}

void sendWriteMultipleHoldingReg(UART_HandleTypeDef* huart, uint8_t slaveID,uint16_t startingReg, uint8_t regNum,uint16_t *values) {

	uint8_t requestFrame[7+MAX_WRITE_REGS*2];

	requestFrame[0] = slaveID;  // slave address
	requestFrame[1] = WRITE_HOLDING_REGs; // 0x10 // 16 Function code for Write multiples Registers

	requestFrame[2] = (startingReg >> 8) & 0xff;
	requestFrame[3] = startingReg & 0xff;

	requestFrame[4] = 0x00;
	requestFrame[5] = regNum;

	requestFrame[6] = regNum * 2;  // Byte Count = 2 bytes for register

	for(uint8_t regCounter=0;regCounter<regNum; regCounter++){

		requestFrame[7+ regCounter*2] = (values[regCounter] >> 8) & 0xff;
		requestFrame[8+ regCounter*2] = values[regCounter] & 0xff;
	}

	uint8_t payloadLen =  7+ regNum * 2;
	sendModBusRequest(huart, requestFrame, payloadLen);
}
