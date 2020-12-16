#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>
#include "tcpserver.h"

int tcp_server(u_short port) {
	int count = 0;	// count the number of clients
	SOCKET ls = socket(AF_INET, SOCK_STREAM, 0);
	if (ls == INVALID_SOCKET) {
		printf("socket is not ready\n");
		return 1;
	}
	printf("socket is ready\n");
	SOCKADDR_IN sa; //server address for binding
	memset(&sa, 0, sizeof(sa));		// memset( void * ptr, int value, size_t num) function sets the first [num] bytes of the block of memory
									// pointed by [ptr] to the specified [value].
	sa.sin_family = AF_INET;	//specify IPv4
	sa.sin_port = htons(port); // host to network short int 
	sa.sin_addr.s_addr = htonl(INADDR_ANY);		//host to network long int 
	int br = bind(ls, (SOCKADDR*)&sa, sizeof(sa));
	if (br == SOCKET_ERROR) {	// if bind is not ok
		printf("bind() fails!\n");
		WSACleanup();
		return 1;
	}
	printf("bind() is ok!\n");

	br = listen(ls, 4);
	if (br == SOCKET_ERROR) {
		printf("listening is not ok\n");
		return 1;
	}
	printf("listening is ok\n");
	
	printf("TCP server is listening on port %d\n\n", port);
	for (;;) {
		SOCKADDR_IN ca; //client address
		int alen = sizeof(ca);	// array length of client address
		SOCKET as = accept(ls, (SOCKADDR*)&ca, &alen);
		if (as == INVALID_SOCKET) {
			printf("accpet returns invalid\n");
			return 1;
		}
		printf("Connection coming from %s:%d\n",inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
		printf("Connection from %s:%d\n",inet_ntoa(ca.sin_addr),ntohs(ca.sin_port));
		char sbuf[1000];
		sprintf(sbuf, "You are the No.%d client", ++count);	// organise a buffer
		br = send(as, sbuf, strlen(sbuf), 0);	// assume return is ok
		closesocket(as);	//close the socket
	}
}