import sys
import serial
import modbus_tk
import modbus_tk.defines as cst
import modbus_tk.modbus_rtu as modbus_rtu
from time import sleep

logger = modbus_tk.utils.create_logger("console")

SLAVE_ID = 7
SERIAL_PORT = '/dev/cu.usbserial-14140'
BAUD_RATE = 115200
VERBOSE_FLAG = True

if __name__ == "__main__":

    #Connect to the slave

    master = modbus_rtu.RtuMaster(serial.Serial(SERIAL_PORT, BAUD_RATE))
    master.set_timeout(5.0)
    master.set_verbose(VERBOSE_FLAG)
    logger.info("connected")

    while True:
        try:

            # print('-----------------------------------------')
            # master.execute(5, cst.READ_COILS, 0, 10)                             #1
            # master.execute(5, cst.READ_DISCRETE_INPUTS, 0, 15)                   #2
            # master.execute(5, cst.READ_HOLDING_REGISTERS, 0, 5)                  #3
            # master.execute(5, cst.READ_INPUT_REGISTERS, 0, 10)                   #4
            # master.execute(5, cst.WRITE_SINGLE_COIL, 2, output_value=1)          #5
            # master.execute(5, cst.WRITE_SINGLE_REGISTER, 8,  output_value=55)    #6
            # master.execute(5, cst.WRITE_MULTIPLE_COILS, 0, output_value=[1] * 8) #15
            # master.execute(5, cst.WRITE_MULTIPLE_REGISTERS, 10,                  #16
            #                output_value=range(5))
            #
            # master.execute(5, cst.WRITE_MULTIPLE_REGISTERS, 0,  # 16
            #                output_value=[2]*5)

            coils = [1,1,0,0,0,0,0,0,1,1]
            master.execute(SLAVE_ID, cst.WRITE_MULTIPLE_COILS, 0, output_value= coils)

            sleep(100/1000)
        except Exception as e:
            logger.error(e)
