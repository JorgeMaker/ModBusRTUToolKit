import logging
import sys

import serial
from modbus_tk import defines as cst
from modbus_tk import modbus_rtu

if __name__ == '__main__':
    log = logging.getLogger()
    log.addHandler(logging.StreamHandler(sys.stdout))
    log.setLevel(logging.DEBUG)

    SLAVE_ID = 7
    SERIAL_PORT = '/dev/cu.usbserial-14140'
    BAUD_RATE = 115200
    VERBOSE_FLAG = True

    server = modbus_rtu.RtuServer(serial.Serial(SERIAL_PORT, BAUD_RATE))
    server.set_verbose(VERBOSE_FLAG)
    server.start()

    slave = server.add_slave(SLAVE_ID)

    slave.add_block("HOLDING_REGISTERS", cst.HOLDING_REGISTERS, 0, 10)
    holding_register_values = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    slave.set_values("HOLDING_REGISTERS", 0, holding_register_values)

    slave.add_block("COILS", cst.COILS, 0, 24)
    coil_values = [1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1]
    slave.set_values("COILS", 0, coil_values)

    slave.add_block("INPUT_REGISTERS", cst.ANALOG_INPUTS, 0, 10)
    analog_register_values = (55, 99, 88, 77, 66, 55, 44, 33, 22, 11)
    slave.set_values("INPUT_REGISTERS", 0, analog_register_values)

    slave.add_block("DISCRETE_INPUTS", cst.DISCRETE_INPUTS, 0, 24)
    discrete_inputs = [1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                       1, 1, 1, 1, 1]
    slave.set_values("DISCRETE_INPUTS", 0, discrete_inputs)
