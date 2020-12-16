#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>
#include "tcpclient.h"

int tcp_client(char* ip, u_short port) {
	SOCKET ss = socket(AF_INET, SOCK_STREAM, 0);
	if (ss == INVALID_SOCKET) {
		printf("socket is not ready\n");
		return 1;
	}
	printf("Client socket is ready\n");
	SOCKADDR_IN sa; //server address for binding
	sa.sin_family = AF_INET; //2
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = inet_addr(ip);
	
	int ret = connect(ss, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) {
		printf("Connection is failed. Please check remote ip and port\n");
		return 1;
	}
	printf("Connection to the server is ok\n");
	while (1) {
		char rbuf[BUF];
		ret = recv(ss, rbuf, BUF, 0); // recv() receive
		if (ret > 0) {
			rbuf[ret] = '\0';
			printf("Received: %s\n",rbuf);
			break;	// once got received message, while(1) stops
		}
	}
	closesocket(ss);
	return 0;
}