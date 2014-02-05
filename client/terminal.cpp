#include "terminal.h"

using namespace std;

Terminal::Terminal() {};

Terminal::Terminal(SOCKET s) {
	transfer = Transfer(s);
}

/**
 * Starts the Terminal running & waiting for input
 */
void Terminal::run() {

	// Set current directory
	if( ! _getcwd(directory, sizeof(directory)) ) {
		
		// If we can't set the current directory, quit
		cout << "ERROR: could not retrieve working directory. \n\r Quitting..";
		Sleep(1000);
		exit(0);
	}

	// Set directory to files folder
	strcat(directory, "\\files");

	// Terminal ready
	cout << "\n\r Ready.. \n\r\n\r";

	// Wait for commands until quit
	while(true) {

		// Wait for input
		char input[128] = "";
		cin >> input;
		cout << "\n\r";

		// Process input
		if( ! process(input) ) {
			//cout << "input not understood!  type 'help' for instructions :) \n\r";
		}
		
		cout << "\n\r";
	}
}

/**
 * Processes input received from the Terminal.
 *
 * returns:	- false if the input is not understood
 *			- true otherwise, after the input has been processed
 */
bool Terminal::process(const char * input) {
	
	// If the user wants to quit
	if(strcmp(input, "quit") == 0) { quit(); }

	// If the user needs help
	else if(strcmp(input, "help") == 0) { showHelp(); }

	// If the user wants to list local resources
	else if(strcmp(input, "list_local") == 0) { listLocal(); }

	// If the user wants to list remote resources
	else if(strcmp(input, "list_remote") == 0) {listRemote(); }

	// If the user wants to send a file
	else if(strcmp(input, "put") == 0) {putFile(); }

	// If the user wants to get a file
	else if(strcmp(input, "get") == 0) {getFile(); }

	// If user wants to delete a file
	else if(strcmp(input, "delete_local") == 0) {deleteLocal(); }

	// If user wants to delete a remote file
	else if(strcmp(input, "delete_remote") == 0) {deleteRemote(); }

	// If the input is not understood
	else return false;
}

/**
 * Quits the application
 */
void Terminal::quit() {

	// Say bye :)
	cout << "Bye!";
	Sleep(1000);

	// Exit program
	exit(0);
}

/**
 * Lists local files
 */
void Terminal::listLocal() {

	// Print initial message
	cout << "Files in directory \"" << directory << "\": \n\r";

	int count = 0;

	// Open directory
	if ((dir = opendir(directory)) != NULL) {
		  
		// Iterate through the directory
		while ((dirEntry = readdir(dir)) != NULL) {

			// Check if entry is a file or folder
				
			int isDir = S_ISDIR(dirEntry->d_type);
				
			// If it is not a folder
			if(!isDir) {

				count++;

				// Print the name
				cout << "- " << dirEntry->d_name << "\n\r";
			}
		}

		closedir (dir);

		if(count == 0) cout << "No files found \n";
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
 * Display help to the user
 */
void Terminal::showHelp() {

	cout << "Operations: \n\r";
	cout << " quit - quit the application \n\r";
	cout << " list_local - list files in the local directory \n\r";
}


/**
 * Lists remote files
 */
void Terminal::listRemote() {

	// Send the list command to the server
	transfer.sendMessage("list");

	// Receive file list
	char response[128];
	strcpy(response, transfer.receiveMessage());

	// Check if the file list is empty
	if(strcmp(&response[0], ";") == 0) {
		cout << "No files on server";
	}

	// If there are files
	else {

		cout << "Files on remote server: \n\r";
		
		// Convert the response to a string
		stringstream ss;
		string fileString;
		ss << response;
		ss >> fileString;

		// Find any occurrences of ; and extract the string
		string delimiter = ";";
		size_t pos = 0;
		string token;
		while((pos = fileString.find(delimiter)) != string::npos) {
			token = fileString.substr(0, pos);
			cout << "- " << token << "\n\r";
			fileString.erase(0, pos + 1);
		}
	}

	cout << "\n\r";
}

/**
 * Sends a file to the server
 */
void Terminal::putFile() {

	// Filestream for the file
	FILE *stream;

	// Get name of file to send
	cout << "Enter the name of the file to send:\n";
	char fileName[128] = "files\\";
	char inStream[128];
	cin >> inStream;
	strcat(fileName, inStream);
	

	// If we can open the file
	if((stream = fopen(fileName, "rb")) != NULL) {
		
		cout << "file opened\n";

		// Send the file
		transfer.sendFile(stream, fileName);

		// Close the filestream
		fclose(stream);
	} 

	// If we couldn't open the file
	else {
		cout << "File could not be opened\n\n";
	}

}

/**
 * Gets a file from the server
 */
void Terminal::getFile() {

	// Get name of file to get
	cout << "Enter the name of the file to get from the server:\n";
	char fileName[128] = "";
	cin >> fileName;

	// Send the header
	char header[128] = "get;filename:";
	strcat(header, fileName);
	strcat(header, ";");
	transfer.sendMessage(header);

	// Wait for a response
	char response[128];
	strcpy(response, transfer.receiveMessage());
	stringstream ss;
	string responseString;
	ss << response;
	ss >> responseString;

	// If failed to open file
	if(strcmp(response, "could not open file") == 0) {

		cout << "File not found \n";
	}

	// If file was found
	else if(responseString.substr(0, 3).compare("put") == 0) {

		// Decode request
		int numPackets, lastPacketSize;
		string filename;

		// Find any occurrences of ; and extract the string
		string delimiter = ";";
		size_t pos = 0, pos2 = 0;
		string token, param, value;
		while((pos = responseString.find(delimiter)) != string::npos) {
			token = responseString.substr(0, pos);

			// Ignore put 
			if(token.compare("put") != 0) {

				// Divide at :
				pos2 = responseString.find(":");
				param = responseString.substr(0, pos2);
				value = responseString.substr(pos2+1, token.size()-pos2-1);

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

			responseString.erase(0, pos + 1);
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
}

/**
 * Deletes a file
 */
void Terminal::deleteLocal() {

	// Get the name of the file to delete
	char base[128] = "files\\";
	char filename[128];
	cout << "Type the name of the file to delete: \n";
	cin >> filename;
	strcat(base, filename);

	// Attempt to delete the file
	int result = remove(base);
	if(result != 0) {
		cout << "File could not be deleted. \n";
	}

	// If the file couldn't be deleted
	else {
		cout << "File deleted.\n";
	}
}

/**
 * Deletes a remote file
 */
void Terminal::deleteRemote() {

	// Get the name of the file to delete
	char filename[128];
	cout << "Type the name of the file to delete: \n";
	cin >> filename;

	// Send header
	char header[128] = "delete;filename:";
	strcat(header, filename);
	strcat(header, ";");
	transfer.sendMessage(header);

	// Wait for response
	char response[128];
	strcpy(response, transfer.receiveMessage());

	// If the file was successfully deleted
	if(strcmp(response, "ok") == 0) {
		cout << "file deleted \n";
	}

	// If the file was not successfully deleted
	else {
		cout << response << "\n";
	}
}