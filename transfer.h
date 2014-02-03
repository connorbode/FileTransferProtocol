#ifndef TRANSFER_H
#define TRANSFER_H
#pragma comment( linker, "/defaultlib:ws2_32.lib" )
#include <WinSock2.h>
#include <iostream>
#include <sstream>
#include <string>

class Transfer {
public:

	// variables
	SOCKET s;
	int ibufferlen;
	char szbuffer[128];
	int ibytessent;
	int ibytesrecv;
	int PACKET_SIZE;
	int HEADER_LENGTH;

	// methods
	Transfer::Transfer();
	Transfer::Transfer(SOCKET);
	bool sendMessage(char*);
	bool sendMessage2(char*);
	bool sendFile(FILE*, std::string);
	void receiveFile(FILE*, int, int);
	char* receiveMessage();
	char* receiveMessage2();
};

#endif