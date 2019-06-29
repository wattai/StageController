# -*- coding: utf-8 -*-
"""
Created on Sat Jun 29 21:45:56 2019

@author: wattai
"""

from socket import socket, AF_INET, SOCK_DGRAM, SOCK_STREAM

HOST = '127.0.0.1'
PORT = 5000
ADDRESS = "127.0.0.1"  # address to send.


def run_by_udp():
    with socket(AF_INET, SOCK_DGRAM) as s:
        # for broad casting.
        # s.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)
        while True:
            msg = input("> ")
            # receive message.
            s.sendto(msg.encode(), (ADDRESS, PORT))


def run_by_tcpip():
    with socket(AF_INET, SOCK_STREAM) as s:
        # set server.
        s.connect((HOST, PORT))
        while True:
            msg = input("> ")
            # send the message to the server.
            s.sendall(msg.encode())
            # buffer size is 1024.
            data = s.recv(1024)
            print(data.decode())


if __name__ == '__main__':
    # run_by_udp()
    run_by_tcpip()
