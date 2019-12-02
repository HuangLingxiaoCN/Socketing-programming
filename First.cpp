// first task of socket programming
// ask the user to enter a dotted decimal IPv4 address,
// you program will verify this is a legal IPv4 address
// and tell the user which class it belongs to

#include <stdio.h>
#include <string.h>
#include <stdlib.h> //for atoi()
#include <ctype.h>	//for isdigit()
#include <WinSock2.h>	// for related socket functions

//check the endianness of the system
union endianness {
	u_long a;	// unsigned long
	u_char b[4];	// unsigned char

};

//check the validity of one segment of ip address
int addr_check(char** addrv);

// the user gives address as command line argument
int main(int argc, char** argv) //argc == argument count    argv == argument value 
{
	// Determine endianness of the system
	union endianness e;
	e.a = 0x10000001;	// 4 bytes
	if (e.b[0] == 0x10)	printf("Big endian\n");
	else printf("Little endian\n");

	if (argc != 2) { // Determine if two arguments are given (file ip_addr)
		printf("Usage: %s dotted decimal_addr\n", argv[0]);
		return 1;
	}
	int f;
	f = addr_check(argv);	// Use addr_check function to check the validity of given ip address
	if (f != 1) {
		printf("Ip address error.\n");
		return 1;
	}

	char part1[4] = "\0"; //initialize the string as 0000

	//stop when '.' is met
	for (int i = 0; ; i++) {
		if (argv[1][i] >= '0' && argv[1][i] <= '9')
			part1[i] = argv[1][i];
		if (argv[1][i] == '.') break;
	}	//for
//	printf("part1 is %s\n", part1);

	int p1v = atoi(part1);	//ascii to integer

	if (p1v >= 0 && p1v <= 127)	printf("Address is class A\n");
	else if (p1v >= 128 && p1v <= 191) printf("Address is class B\n");
	else if (p1v >= 192 && p1v <= 223) printf("Address is class C\n");
	else printf("Other classes\n");

	return 0;
}// main()

int addr_check(char** addrv)
{
	int i = 0, flag;
	int str_len = strlen(addrv[1]);
	//1. ip address is minimum 7 characters and maximum 15 characters long.
	if (str_len >= 7 && str_len <= 15) {
		//2. count the occurance of '.' should be 3 and no two dots should be consecutive.
		int dot_count = 0;
		while (addrv[1][i] != '\0') {
			if (addrv[1][i] == '.')
				if (addrv[1][i + 1] != '.')
					dot_count++;
			i++;
		}// while

		if (dot_count == 3) {
			i = 0;
			int alpha_count = 0;

			//3. the string only contains '.' and numbers 0 - 9.
			for (; ; i++) {
				if (addrv[1][i] == '\0') break;			//Caution: end of string '\0' is not digit or dot.
				if (isdigit(addrv[1][i]) || addrv[1][i] == '.')
				{
					continue;
				}
				else alpha_count++;

			}// for
			if (alpha_count == 0) {
				//4. the number between each dot shoule be 0 - 255 
				char num[100][100];
				int err_count = 0, k;
				i = 0;
				for (int j = 0; j < 4; j++) {	//four numbers in an ip addr
					k = 0;
					while (addrv[1][i] != '\0') {
						if (addrv[1][i] == '.') {
							i++;
							break;
						}
						num[j][k] = addrv[1][i];
						i++;
						k++;
					}
					if (atoi(num[j]) < 0 || atoi(num[j]) > 255)	err_count++;
				}// for
				if (err_count == 0) {
					//5. The starting and ending should not be dots
					if (!(addrv[1][0] == '.') && !(addrv[1][str_len - 1] == '.')) flag = 1;
					else flag = 0;
				}// if-else err_count

				else flag = 0;

			}// if-else alpha_count
			else flag = 0;

		}// if-else dot_count
		else flag = 0;

	}// if-else str_len
	else flag = 0;

	return flag;
}