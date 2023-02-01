This library implements ModBus TPU on µC STM32 offering the a set
of functions that allow you to build both a Slave and a Master of the
ModBus protocol.


To create a Master after configuring a UART you must activate the function
from callback.

// Enable interrupt for the first reception
HAL_GPIO_WritePin(TX_CTRL_GPIO_Port, TX_CTRL_Pin, GPIO_PIN_RESET);

Below you can see the example of function calls from the execution loop:

main funtion:

```c++

ModbusSlave modbusSlave(7, &huart2, TX_CRTL_GPIO_Port, TX_CRTL_Pin);
modbusSlave.begin();


```
main loop:
```c++
modbusMaster.sendReadHoldingRegsRequest(7, 0, 5);
modbusMaster.sendWriteSingleHoldingReg(7, 10,110);
uint16_t values[10] = {10,20,30,40,50,60,80,90,100,110};
modbusMaster.sendWriteMultipleHoldingReg(7, 0, 10, values);
```
To receive to receive the answers we have to implement the following
method:

```c++
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
	if (rxFrame[0] == SLAVE_ID) {
		uint8_t opCode = rxFrame[1];
    switch (opCode) {
      case READ_COILs:
      ......
      ......
      default:
				break;
    }
    // Enable interrupt for the next reception
    HAL_UARTEx_ReceiveToIdle_IT(&huart,rxFrame, 32);
}
```

To create a Slave after configuring a UART you must activate the function
from callback ad then the following method to handle each request coming from
the Master.  

main function:
```c++
ModbusSlave modbusSlave(7, &huart2, TX_CRTL_GPIO_Port, TX_CRTL_Pin);
modbusSlave.begin();
```
To receive to receive the answers we have to implement the following
method:
```c++
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {

	uint8_t* rxBuffer = ModbusMaster::getRxBufferReferece();

	uint8_t opCode = rxBuffer[1];

	ModBusReadMutilpleResult readMultipleResult;
	ModBusWriteMultipleResult writeMultipleResult;
	ModBusWriteSingleResult   writeSingleResult;

	switch (opCode) {
		case READ_COILs:
			readMultipleResult = ModbusMaster::handleReadCoilsResult();
      ......
			......

			default:
				break;
		}
		HAL_UARTEx_ReceiveToIdle_IT(ModbusMaster::getUARTDecriptorRef(),ModbusMaster::getRxBufferReferece(),255);
	    HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
	}
```
