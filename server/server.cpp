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

	cout << "\nPUT Request\n";

	// Decode request
	int numPackets, lastPacketSize;
	string filename;

	// Find any occurrences of ; and extract the string
	string delimiter = ";";
	size_t pos = 0, pos2 = 0;
	string token, param, value;
	while((pos = request.find(delimiter)) != string::npos) {
		token = request.substr(0, pos);

		// Ignore put 
		if(token.compare("put") != 0) {

			// Divide at :
			pos2 = request.find(":");
			param = request.substr(0, pos2);
			value = request.substr(pos2+1, token.size()-pos2-1);

			// If numPackets
			if(param.compare("num_packets") == 0) {
				numPackets = atoi(value.c_str());
				printf("Number of packets: %ld. \n", numPackets);
			}

			// If lastPacketSize
			else if(param.compare("last_packet_size") == 0) { 
				lastPacketSize = atoi(value.c_str());
				printf("Last packet size: %ld. \n", lastPacketSize);
			}

			// If filename
			else if(param.compare("filename") == 0) {
				filename = value;
				cout << filename << "\n";
			}
		}

		request.erase(0, pos + 1);
	}

	// Establish connection to file
	FILE *stream;
	if((stream = fopen(filename.c_str(), "wb")) != NULL) {

		// Receive file
		transfer.receiveFile(stream, numPackets, lastPacketSize);

		// Close the stream
		fclose(stream);
	}

	// If we couldn't create file
	else {

		cout << "Couldn't create file\n\n";
	}
	
	
}