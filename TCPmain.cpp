// This TCP server will open a port specified by the user, and accepts
// client connections. Once a client connection comes in, the server simply
// sends a message to tell the client the sequence of this connection.
#pragma comment(lib, "ws2_32.lib")	//
#include <stdio.h>
#include <WinSock2.h>
#include "tcpserver.h"

int main(int argc, char** argv){
	if (argc != 2) { // Determine if two arguments are given (file ip_addr)
		printf("Usage: %s [port_number]\n", argv[0]);
		return 1;
	}

	WSADATA ws;
	int ret = WSAStartup(MAKEWORD(2, 2), &ws);
	if (ret) {	// if ret is not zero, something goes wrong.
		printf("WinSock API cannot start!\n");
		return 1;
	}
	printf("WinSock API is ready!\n");
	tcp_server(atoi(argv[1]));
	WSACleanup();
	return 0;
}