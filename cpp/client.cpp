// TCP/IP client for XYZ-Rotation controller

#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 5000 // port number

int main() {
	// IP address，port number，socket，constructor: sockaddr_in 
	char destination[32];
	int dstSocket;
	struct sockaddr_in dstAddr;

	// parameters
	char buffer[1024];

	// for windows
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);

	// enter IP address to send
	printf("Please enter IP address of server: ");
	// enter command
	scanf_s("%s", destination, 32);

	// set constructor: sockaddr_in 
	memset(&dstAddr, 0, sizeof(dstAddr));
	dstAddr.sin_port = htons(PORT);
	dstAddr.sin_family = AF_INET;
	InetPtonA(dstAddr.sin_family, destination, &dstAddr.sin_addr.s_addr);

	// generate socket
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	// make connection
	if (connect(dstSocket, (struct sockaddr*) & dstAddr, sizeof(dstAddr))) {
		printf("I could not connect with: %s\n", destination);
		return(-1);
	}
	printf("I connected with %s\n", destination);
	printf("Please enter your command here: \n");

	while (1) {
		scanf_s("%s", buffer, 1024);
		if (!strcmp(buffer, "quit") || !strcmp(buffer, "exit")) {
			break;
		}
		// send packet
		send(dstSocket, buffer, strlen(buffer), 0);

		// initialize memory
		memset(buffer, 0, sizeof(buffer));

		// receive packet
		recv(dstSocket, buffer, 1024, 0);
		printf("response: %s\n\n", buffer);
	}

	// close socket on this device
	closesocket(dstSocket);
	WSACleanup();
	return(0);
}
