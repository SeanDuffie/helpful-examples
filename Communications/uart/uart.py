""" uart.py

    This example is scrapped from the serial connection between the arduino and the Raspberry Pi
    in my Synthetic Garden Project. It uses the arduino to communicate with all of the analog
    sensors, then when prompted by the RPi controller, it will output the current sensor readings
    over the Serial connection.

    The code below demonstrates the RPi side of the program, which it 

"""
import time

import serial
from serial.tools import list_ports

PORT_LIST = list(list_ports.comports())
for p in PORT_LIST:
    print("  - ", p.device)
print()

# Setting up Serial connection
ser = serial.Serial(
    # port='/dev/ttyACM0', # use this to manually specify port
    port=PORT_LIST[0].device,
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=10
)

# Send out requests until a response is received
while ser.in_waiting == 0:
    print("Waiting for sensors...")
    ser.write('1'.encode('utf-8'))
    time.sleep(.5)
print()

# Receive the Response from the sensors
SENSOR_ARR = list()
print("Reading Response...")
FEEDBACK = ser.readline()
SENSOR_READ = FEEDBACK.decode("Ascii")
SENSOR_ARR = SENSOR_READ.split(",")
print(SENSOR_ARR)
print()