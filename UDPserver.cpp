// this program will be working as UDP server. It opens a (registered) port
// which is given as command argument from the user. This program will display
// datagram from clients wirh clients' IP address, and bounce back the message
// to the clients with server name.

#pragma comment(lib, "ws2_32.lib")	//invoke the library
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>

int main(int argc, char* argv[]) {
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
	printf("WinSock is ready!\n");

	//We should start socket programming ...
	SOCKET server = socket(AF_INET, SOCK_DGRAM, 0);	// Family, Type and Protocol. Protocol will be automatically selected according to type.
	if (server == INVALID_SOCKET) {	// If server is not ok
		printf("Socket cannot be created.\n");;
		WSACleanup();
		return 1;
	}
	printf("Socket is created!\n");

	SOCKADDR_IN sa; //server address for binding
	memset(&sa, 0, sizeof(sa));		// memset( void * ptr, int value, size_t num) function sets the first [num] bytes of the block of memory
									// pointed by [ptr] to the specified [value].
	sa.sin_family = AF_INET;	//specify IPv4
	sa.sin_port = htons(atoi(argv[1])); // host to network short int 
	sa.sin_addr.s_addr = htonl(INADDR_ANY);		//host to network long int 
	int br = bind(server, (SOCKADDR*)&sa, sizeof(sa));
	if (br == SOCKET_ERROR) {	// if bind is not ok
		printf("bind() fails!\n");
		WSACleanup();
		return 1;
	}
	printf("bind() is ok!\n");

	char sname[50], rbuf[1000], sbuf[1000];	//server name, received buff, sended buffer
	gethostname(sname, sizeof(sname));	//gethostname() is a build-in function to get host name
	printf("UDP server %s is waiting on port %d\n\n", sname, atoi(argv[1]));

	//infinite loop similar to while(1)
	for (;;) {
		SOCKADDR_IN rmt;
		int alen = sizeof(rmt);
		int rlen = recvfrom(server, rbuf, sizeof(rbuf), 0, (SOCKADDR *)&rmt, &alen);	//received length
		if (rlen > 0) {
			//NOTE: string terminator '\0' is not sent in C language.
			//So you need to add it by yourself.
			rbuf[rlen] = '\0';
			printf("%s:%d >> %s\n", inet_ntoa(rmt.sin_addr), ntohs(rmt.sin_port), rbuf);
			sprintf(sbuf, "%s >> %s", sname, rbuf);	// sprintf is a standard function that composes a string with the same text that could be printed
													// by printf. sprintf returns strlen.
			sendto(server, sbuf, strlen(sbuf), 0, (SOCKADDR *)& rmt, alen);

		}
	}// end for(;;)
	WSACleanup();
	return 0;
}