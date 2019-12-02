// UDP client. This program will ask user to enter the ip address and port number
// of a server  from command argument, and send user strings to the server.
// The program stops when the user enters QUIT as input.

#pragma comment(lib, "ws2_32.lib")	//invoke the library
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {	// determine if the arguments given are 3. (file ip_addr port)
		printf("Usage: %s [ip address] [port_number]\n", argv[0]);
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
	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;	//specify IPv4
	sa.sin_addr.s_addr = inet_addr(argv[1]);	// The inet_addr() function shall convert the string to standard IPv4 dotted decimal notation.
	sa.sin_port = htons(atoi(argv[2]));	// host to network short int 
	printf("Welcome to UDP client. We are sending messages to %s:%d\n", argv[1], atoi(argv[2]));
	printf("To end the program, enter QUIT\n\n");
	while (1) {	// infinite while loop until user type QUIT
		char rbuf[1000], sbuf[1000];
		int alen = sizeof(sa);
		printf("Message >> ");
		fgets(sbuf,1000,stdin);	//fgets(char *str, int n, FILE *stream) gets '\n' (ENTER) before '\0'(string terminator)
		sbuf[strlen(sbuf)-1] = '\0';	// change the '\n' to '\0'
		//scanf("%s", sbuf);
		if (strcmp(sbuf, "QUIT") == 0)	break;
		sendto(server, sbuf, strlen(sbuf), 0, (SOCKADDR*)&sa, alen);
		int rlen = recvfrom(server, rbuf, sizeof(rbuf), 0, (SOCKADDR*)&sa, &alen);
		if (rlen > 0) {
			rbuf[rlen] = '\0';
			printf("\t%s\n", rbuf);
		}
	} // end while(1)

	printf("Thank you for using the service!\n");
	WSACleanup();
	return 0;
}	//end of program