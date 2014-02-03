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

				// Print the name
				cout << "- " << dirEntry->d_name << "\n\r";
			}
		}

		closedir (dir);
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
	char fileName[128] = "";
	cin >> fileName;

	// If we can open the file
	if((stream = fopen(fileName, "r+b")) != NULL) {
		
		cout << "file opened\n";

		// Send the file
		transfer.sendFile(stream);

		// Close the filestream
		fclose(stream);
	} 

	// If we couldn't open the file
	else {
		cout << "File could not be opened\n\n";
	}

}