#ifndef TRANSFER_H
#define TRANSFER_H
#pragma comment( linker, "/defaultlib:ws2_32.lib" )
#include <WinSock2.h>
#include <iostream>

class Transfer {
public:
	SOCKET s;
	int ibufferlen;
	char szbuffer[128];
	int ibytessent;
	int ibytesrecv;

	Transfer::Transfer();
	Transfer::Transfer(SOCKET);
	bool sendMessage(char*);
	char* receiveMessage();
};

#endif