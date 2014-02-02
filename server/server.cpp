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

	// Set current directory
	if( ! _getcwd(directory, sizeof(directory)) ) {
		
		// If we can't set the current directory, quit
		cout << "ERROR: could not retrieve working directory. \n\r Quitting..";
		Sleep(1000);
		exit(0);
	}

	// Loop until close
	while (true) {

		// Receive a message
		char message[128];
		strcpy(message, transfer.receiveMessage());

		// Decode message
		if(strcmp(message, "list") == 0) { list(); }
	}
}

/**
 * Lists files on the server
 */
void Server::list() {

	// Open directory
	if ((dir = opendir(directory)) != NULL) {
		  
		// Iterate through the directory
		while ((dirEntry = readdir(dir)) != NULL) {

			struct stat st;

			// Check if entry is a file or folder
			stat(dirEntry->d_name, &st);
			  
			// If it is a folder
			if(S_ISDIR(st.st_mode)) {

				// Do nothing.
			}

			// If it is a file
			else {

				cout << "sent " << dirEntry->d_name << "\n\r";

				// Send the name to the client
				transfer.sendMessage(dirEntry->d_name);
			}
		}

		closedir (dir);
		transfer.sendMessage("done");
	} 
		
	// Failed to open directory
	else {
		  
		// say bye and quit!
		cout << "Failed to open directory :( \n\r Quitting..";
		Sleep(1000);
		exit(0);
	}

	cout << "\n\r";
}