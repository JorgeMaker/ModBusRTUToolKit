/*******************************
 *      modbusDevice.h
 *
 *  Created on: 1 feb. 2023
 *   Author: JorgeMaker
*******************************/

#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_

#include "main.h"
#include "stm32f1xx_hal_uart.h"


#define MAX_TX_FRAME 255
#define MAX_RX_FRAME 255
#define NUM_OF_HOLDING_REGS 100

typedef enum {
	MODBUS_OK = 	0x01,
	MODBUS_ERROR =	0x00
}ModBusResult;

typedef enum{
	ON  = 0x01,
	OFF = 0x00
}binaryDataValue;


typedef enum {
	READ_COILs = 			0x01,
	READ_DISC_INPUTs =		0x02,
	READ_HOLDING_REGs = 	0x03,
	READ_INPUT_REGs = 		0x04,

	WRITE_SINGLE_COIL = 	0x05,
	WRITE_SINGLE_REG = 		0x06,
	WRITE_MULT_COILs = 		0x0F, // 15
	WRITE_HOLDING_REGs =	0x10  // 16
} modbusOpCode;

typedef enum {
	ILEGAL_FUCTION 	=   	0x01,
	ILLEGAL_DATA_ADDRESS=   0x02,
	ILLEGAL_DATA_VALUE = 	0x03
} modbusExceptionCode;

/*
 * Class: ModBusDevice
 *
 * This class is used to create a ModBus device object and communicate with the device.
 *
 * Attributes:
 *  rxFrame      - A static array to store the received frame.
 *  huart        - A static pointer to UART_HandleTypeDef to access the UART descriptor.
 *  txFrame      - An array to store the transmitted frame.
 *  enTXPinPort  - A GPIO_TypeDef pointer to the enable port.
 *  enTXPinNumber- The number of the enable pin.
 */

class ModBusDevice{

	public:
		ModBusDevice(UART_HandleTypeDef* serailPort,GPIO_TypeDef*  enablePort, uint16_t pinNumber);
		ModBusResult begin();
		ModBusResult stop();
		static uint8_t* getRxBufferReferece();
		static UART_HandleTypeDef* getUARTDecriptorRef();

	protected:
		static uint8_t rxFrame[MAX_RX_FRAME];
		static uint8_t txFrame[MAX_TX_FRAME];
		static UART_HandleTypeDef* huart;
		static GPIO_TypeDef* enTXPinPort;
		static uint16_t enTXPinNumber;

		static uint16_t calcCRC16ModBus(uint8_t *buffer, uint8_t u8length);
		static uint8_t encodeBitsIntoByte (uint8_t *bitToEncode, uint8_t len);
		static uint8_t encodeCoils(uint8_t* coilValues, uint8_t coilsNum, uint8_t* output_array);
		static void copyArrayRangeUpTo8(uint8_t* origin, uint8_t* destination, uint8_t start,  uint8_t end);
		void extractBinaryReceivedData(uint8_t len, uint8_t *receivedData,uint8_t *extractedBits);
		static ModBusResult sendModBusRequest(uint8_t len);

};
#endif /* INC_MODBUS_H_ */
