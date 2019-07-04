// TCP/IP client for XYZ-Rotation controller
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 5000 // port number

class StageController
{

public:
	char destination[32];
	int dstSocket;
	struct sockaddr_in dstAddr;

	// parameters
	char buffer[1024];

	bool Initialize();
	std::string SendCommand(char* command);
	void Close();
};

bool StageController::Initialize() {
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
		return false;  // return(-1);
	}
	else {
		printf("I connected with %s\n", destination);
		printf("Please enter your command here: \n");
		return true;
	}
}

std::string StageController::SendCommand(char* command) {
	// send packet
	send(dstSocket, command, strlen(command), 0);

	// initialize memory
	memset(buffer, 0, sizeof(buffer));

	// receive packet
	recv(dstSocket, buffer, 1024, 0);
	printf("response: %s\n\n", buffer);
	return buffer;
}

void StageController::Close() {
	// close socket on this device
	closesocket(dstSocket);
	WSACleanup();
}

int main() {
	char buffer[1024];
	StageController stage;
	std::string res;

	// initialize a class
	bool ret = stage.Initialize();
	// if the initialization successed
	if (ret) {
		while (1) {
			scanf_s("%s", buffer, 1024);
			// if found some exit command
			if (!strcmp(buffer, "quit") || !strcmp(buffer, "exit")) {
				// close the class, and then terminate
				stage.Close();
				return(-1);
			}
			// send command throught TCP/IP
			res = stage.SendCommand(buffer);
		}
		// else if the initialization did NOT sucess
	}
	else {
		// terminate
		return(-1);
	}
}

/*
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

	// start communication
	while (1) {
		scanf_s("%s", buffer, 1024);
		if (!strcmp(buffer, "quit") || !strcmp(buffer, "exit")){
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
*/