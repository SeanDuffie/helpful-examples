import time

import serial


class UART:
    def __init__(self):
        self.active_ports = self.print_ports()
        self.ser = self.init_serial()

    def print_ports(self):
        """
        """
        PORT_LIST = serial.tools.list_ports.comports()
        print("Current Devices:")
        for p in PORT_LIST:
            print("  - ", p.device)
        print()
        return PORT_LIST

    def init_serial(self):
        # Setting up Serial connection
        ser = serial.Serial(
            # port='/dev/ttyACM0', # use this to manually specify port
            port=self.active_ports[0].device,
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=10
        )

        return ser

    def send_req(self, msg):
        # Send out requests until a response is received
        while self.ser.in_waiting == 0:
            print("Waiting for sensors...")
            self.ser.write(msg.encode('utf-8'))
            time.sleep(.5)
        print()