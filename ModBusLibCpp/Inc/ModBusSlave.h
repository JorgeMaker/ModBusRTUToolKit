/*******************************
 *      modbusSlave.h
 *
 *  Created on: 1 feb. 2023
 *   Author: JorgeMaker
*******************************/
#ifndef INC_MODBUSSLAVE_H_
#define INC_MODBUSSLAVE_H_

#include "ModBusDevice.h"

class ModbusSlave: public ModBusDevice{

	public:
		ModbusSlave(uint8_t slave_ID, UART_HandleTypeDef* serailPort,GPIO_TypeDef*  enablePort, uint16_t pinNumber);

		static ModBusResult handleReadHoldingRegs (uint16_t* holdingRegisterValues);
		static ModBusResult handleReadInputRegs (uint16_t*  inputRegisterValues);
		static ModBusResult handleReadCoils (uint8_t* coilValues);
		static ModBusResult handleReadDiscreteInputs (uint8_t* dicreteInputValues);
		static ModBusResult handleWriteSingleHoldingRegister (uint16_t* holdingRegisterValues);
		static ModBusResult handleWriteMulyipleHoldingRegisters (uint16_t* holdingRegisterValues);
		static ModBusResult handleWriteSingleCoil (uint8_t* coilValues);
		static ModBusResult handleWriteMultipleCoils (uint8_t* coilValues);

		static ModBusResult sendModbusException (modbusExceptionCode exceptionCode);

		static uint8_t getSlaveID();

	private:
		static uint8_t slaveID;
		static void extractFromByte(uint8_t byte, uint8_t bitToExtract,uint8_t* outPutResuls );
		static void unpackCoilsFromBuffer(uint8_t numerOfBytes,uint8_t numberOfCoils,uint8_t* bytesSource, uint8_t* outPutResuls);
};


#endif /* INC_MODBUSSLAVE_H_ */
