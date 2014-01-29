#ifndef SERVER_H
#define SERVER_H

#include "../transfer.h"
#include <iostream>

class Server {

public:
	// methods
	Server();
	Server(SOCKET);
	void run();

	// variables
	Transfer transfer;
};

#endif
