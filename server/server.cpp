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
		stringstream ss;
		string messageString;
		ss << message;
		ss >> messageString;

		cout << "Received message: " << messageString << "\n";

		// Decode message
		if(strcmp(message, "list") == 0) { list(); }
		else if(messageString.substr(0, 3).compare("put") == 0) { put(messageString); }
	}
}

/**
 * Lists files on the server
 */
void Server::list() {

	// Filelist to send
	char fileList[128] = "";

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

				// Append the filename to the list
				strcat(fileList, dirEntry->d_name);
				strcat(fileList, ";");
			}
		}

		// Close the directory
		closedir (dir);

		// Check to see if the filelist was empty
		if(strcmp(fileList, "") == 0) {
			strcat(fileList, ";");
		}

		// Send the fileList back to the client
		transfer.sendMessage(fileList);
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

/**
 * Deals with a put request
 */
void Server::put(string request) {

	// Decode request
	
}