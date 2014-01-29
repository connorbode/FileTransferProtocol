#include "server.h"

using namespace std;

Server::Server() {};

Server::Server(SOCKET s) {
	
	transfer = Transfer(s);
}

/**
 * Starts the server listening for incoming messages
 */
void Server::run() {

	// Loop until close
	while (true) {

		// Receive a message
		cout << transfer.receiveMessage();
	}
}