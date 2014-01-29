#ifndef SERVER
#define SERVER

#include "../transfer.h";

class Server {

public:
	// methods
	Server(SOCKET);
	void run();

	// variables
	Transfer transfer;
}

#endif
