## ModbusToolKit for STM32 Microcontrollers

------------

This repository contains a couple of libraries, one in C and one in C++ for developing ModBus RTU applications using µC STM32 microcontrollers.

------------

Modbus is an industrial communication protocol used to connect electronic devices and allow them to interact with each other. It is an open and standard protocol, and the RTU version is widely used for real-time communications over serial lines.

------------

In order to use the provided code, it is necessary to enable a preferred USART as well as an output GPIO. In the USART port, the interrupt must be activated in order to properly receive and transmit data. This can typically be done by configuring the NVIC (Nested Vector Interrupt Controller).

------------

Also pieces of code are provided that can serve as a guide when using these libraries.
In order to test your application, a couple of Python scripts that act as Master and Slave are provided as part of the toolkit.

------------

##### File structure
+ LICENSE
+ README.md
    + ModBusLibC      ------->   Library for C
    + ModBusLibCpp  -----> Library for C++
    + Test  ---------------> Python scripts for testing
