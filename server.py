# -*- coding: utf-8 -*-
"""
Created on Sat Jun 29 21:43:32 2019

@author: wattai
"""

from socket import socket, AF_INET, SOCK_DGRAM, SOCK_STREAM
import stage_controller

HOST = '127.0.0.1'
PORT = 5000


def run_by_udp(controller):
    # prepare for socket.
    with socket(AF_INET, SOCK_DGRAM) as s:
        s = socket(AF_INET, SOCK_DGRAM)
        # binding.
        s.bind((HOST, PORT))

        while True:
            # receive message.
            msg, address = s.recvfrom(1024)
            print(f"message: {msg}\nfrom: {address}")
            controller.send_command(command=msg.decode())


def run_by_tcpip(controller):
    # AF == IPv4
    # In the case of TCP/IP, we use SOCK_STREAM.
    with socket(AF_INET, SOCK_STREAM) as s:
        # set IPadrress and port.
        s.bind((HOST, PORT))
        # 1 connection.
        s.listen(1)
        # wait until connection will be done.
        while True:
            # Ifsomeone access, set the connection and the address.
            conn, addr = s.accept()
            with conn:
                while True:
                    # receive data.
                    data = conn.recv(1024)
                    if not data:
                        break
                    print('data : {}, addr: {}'.format(data, addr))
                    try:
                        res = controller.send_command(command=data.decode())
                    except TypeError:
                        controller = stage_controller.StageController()
                        res = controller.send_command(command=data.decode())

                    # return the data to client (b -> byte is needed)
                    conn.sendall(', '.join(res).encode())


if __name__ == '__main__':
    controller = stage_controller.StageController()
    # run_by_udp(controller)
    run_by_tcpip(controller)
