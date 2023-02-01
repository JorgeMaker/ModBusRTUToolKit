This library implements ModBus TPU on µC STM32 offering the a set
of functions that allow you to build both a Slave and a Master of the
ModBus protocol.


To create a Master after configuring a UART you must activate the function
from callback.

// Enable interrupt for the first reception
HAL_GPIO_WritePin(TX_CTRL_GPIO_Port, TX_CTRL_Pin, GPIO_PIN_RESET);

Below you can see the example of function calls from the execution loop:
```c++
sendReadHoldingRegsRequest(huart2, 5, 0, 10);
sendReadInputRegsRequest(huart2, 5, 0, 10);
uint8_t coilValues[12] = {ON,ON,ON,ON,ON,ON,ON,ON,ON,ON,ON,ON};
sendWriteMultipleCoils(huart2, 7, 0, 12, coilValues);
sendWriteSingleCoilRequest(huart2, 7, 10, 1);
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

```c++
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
```
