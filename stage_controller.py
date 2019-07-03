# -*- coding: utf-8 -*-
"""
Created on Sat Jun 29 16:56:13 2019

@author: wattai
"""

import serial
import serial.tools.list_ports


class StageController:

    def __init__(self, port=None, baudrate=9600, timeout=0.1,
                 write_timeout=0):
        if port is None:
            port = self.find_com_port()  # find com port.

        self.return_code = b"\r\n"  # assume the delimiter is 'CLRF'.

        self.ser = serial.Serial()
        self.ser.port = port
        self.ser.baudrate = baudrate
        self.ser.timeout = timeout
        self.ser.write_timeout = write_timeout
        self.ser.parity = serial.PARITY_NONE
        self.ser.inter_byte_timeout = None
        dict_setting = self.ser.get_settings()
        print("Setting:\n", dict_setting)

    def send_command(self, command):
        with self.ser as s:
            if s.is_open:
                if type(command) is list:
                    res = self.write_and_readline(s, command)
                else:
                    res = self.write_and_readline(s, [command])
            else:
                raise(ValueError("Serial port is not opened."))
        return res

    def write_and_readline(self, s, command_list):
        res_list = []
        for c in command_list:
            if type(c) is not str:
                message = "The type of an element of 'command_list' is not 'str'."
                raise(TypeError(message))
            data = bytes(c.encode('utf-8')) + self.return_code
            s.write(data)
            print("command: %s" % c)
            res = s.readline().decode().rstrip(self.return_code.decode())
            res_list.append(res)
            print("response: %s" % res)
        return res_list

    def find_com_port(self,):
        coms = serial.tools.list_ports.comports()
        com_list = []
        for com in coms:
            com_list.append(com.device)
        print('Connected COM ports: ' + str(com_list))
        use_port = com_list[0]
        print('Use COM port: ' + use_port)
        return use_port


if __name__ == '__main__':
    controller = StageController(port='COM3')
    # command = ["M:4+P144000", "G:"]
    command = ["H:W"]
    controller.send_command(command=command)
