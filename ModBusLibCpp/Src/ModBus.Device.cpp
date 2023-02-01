/*******************************
 *      modbusDevice.cpp
 *
 *  Created on: 1 feb. 2023
 *   Author: JorgeMaker
*******************************/
#include "../Inc/ModBusDevice.h"

#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"
#include "string.h"
#include "main.h"

// Initialization of static members for class ModBusDevice

UART_HandleTypeDef* ModBusDevice::huart = NULL;
GPIO_TypeDef* ModBusDevice::enTXPinPort = NULL;
uint16_t ModBusDevice::enTXPinNumber = 0;
uint8_t ModBusDevice::txFrame[MAX_RX_FRAME] = { };
uint8_t ModBusDevice::rxFrame[MAX_RX_FRAME] = { };

ModBusDevice::ModBusDevice(UART_HandleTypeDef *serialPort,
		GPIO_TypeDef *enablePort, uint16_t pinNumber) {

	huart = serialPort;
	enTXPinPort = enablePort;
	enTXPinNumber = pinNumber;
}

uint8_t* ModBusDevice::getRxBufferReferece() {
	return rxFrame;
}
UART_HandleTypeDef* ModBusDevice::getUARTDecriptorRef() {
	return huart;
}

ModBusResult ModBusDevice::begin() {
	HAL_GPIO_WritePin(enTXPinPort, enTXPinNumber, GPIO_PIN_RESET);
	// Enable interrupt for the first reception
	if (HAL_UARTEx_ReceiveToIdle_IT(huart, rxFrame, 32) == HAL_OK) {
		return MODBUS_OK;
	} else {
		return MODBUS_ERROR;
	}
}

ModBusResult ModBusDevice::stop() {

	// Stop reception of interrupts for serial port
	if (HAL_UART_AbortReceive_IT(huart) == HAL_OK) {
		return MODBUS_OK;
	} else {
		return MODBUS_ERROR;
	}
}

uint16_t ModBusDevice::calcCRC16ModBus(uint8_t *buffer, uint8_t u8length) {
	unsigned int temp, temp2, flag;
	temp = 0xFFFF;
	for (unsigned char i = 0; i < u8length; i++) {
		temp = temp ^ buffer[i];
		for (unsigned char j = 1; j <= 8; j++) {
			flag = temp & 0x0001;
			temp >>= 1;
			if (flag)
				temp ^= 0xA001;
		}
	}
	// Reverse byte order.
	temp2 = temp >> 8;
	temp = (temp << 8) | temp2;
	temp &= 0xFFFF;
	// the returned value is already swapped
	// crcLo byte is first & crcHi byte is last
	return temp;
}

uint8_t ModBusDevice::encodeBitsIntoByte(uint8_t *bitToEncode, uint8_t len) {
	uint8_t encodedValue = 0;
	int bitCursor;
	for (bitCursor = 0; (bitCursor < 8) && (bitCursor < len); bitCursor++) {
		encodedValue |= (bitToEncode[bitCursor] << (7 - bitCursor));
	}
	return encodedValue;
}

uint8_t ModBusDevice::encodeCoils(uint8_t *coilValues, uint8_t coilsNum,
		uint8_t *output_array) {

	uint8_t coilsCursor = 0;
	uint8_t bytesCounter = 0;
	uint8_t dataToEncode[8] = { };

	while (coilsNum > coilsCursor) {

		memset(dataToEncode, 0, 8);
		uint8_t chunk = 0;
		if (coilsCursor + 8 <= coilsNum) {
			chunk = 8;
		} else {
			chunk = coilsNum - coilsCursor;
		}
		copyArrayRangeUpTo8(coilValues, dataToEncode, coilsCursor,
				coilsCursor + chunk);
		uint8_t currentOutputByte = encodeBitsIntoByte(dataToEncode, chunk);
		output_array[bytesCounter] = currentOutputByte;
		bytesCounter++;
		coilsCursor = coilsCursor + 8;
	}
	return bytesCounter;
}

void ModBusDevice::copyArrayRangeUpTo8(uint8_t *origin, uint8_t *destination,
		uint8_t start, uint8_t end) {

	size_t copy_size = end - start;
	if (copy_size > 8) {
		copy_size = 8;
	}
	memcpy(destination, origin + start, (size_t) copy_size);
}

/**
 * @brief extractBinaryReceivedData is a function that takes an array of received data and extracts the individual bits from each byte,
 * storing them in another array.
 *
 * @param len: the length of the receivedData array
 * @param receivedData: the array containing the data to be extracted
 * @param extractedBits: the array where the extracted bits will be stored
 */
void ModBusDevice::extractBinaryReceivedData(uint8_t len, uint8_t *receivedData,uint8_t *extractedBits) {
	uint8_t rxDataCursor = 0;
	for (uint8_t byteCursor = 0; byteCursor < len; byteCursor++) {
		// Obtain each received byte containing
		uint8_t currentByte = receivedData[3 + byteCursor];
		for (int i = 0; i < 8; i++) {
			extractedBits[rxDataCursor] = (currentByte & (1 << i)) >> i;
			rxDataCursor++;
		}
	}
}

ModBusResult ModBusDevice::sendModBusRequest(uint8_t len) {

	uint16_t crc = calcCRC16ModBus(txFrame, len);
	txFrame[len] = (crc >> 8) & 0xFF;    // CRC HIGH
	txFrame[len + 1] = crc & 0xFF;       // CRC LOW

	HAL_GPIO_WritePin(enTXPinPort, enTXPinNumber, GPIO_PIN_SET);

	if (HAL_UART_Transmit(huart, txFrame, len + 2, 1000) == HAL_OK) {
		HAL_GPIO_WritePin(enTXPinPort, enTXPinNumber, GPIO_PIN_RESET);
		return MODBUS_OK;
	} else {
		HAL_GPIO_WritePin(enTXPinPort, enTXPinNumber, GPIO_PIN_RESET);
		return MODBUS_ERROR;
	}
#if defined(DELAY_PRECCESING_FLAG)
	HAL_Delay(PROCCESING_PAUSE);
#endif
}
