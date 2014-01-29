#include "terminal.h"

using namespace std;

Terminal::Terminal() {
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
			cout << "input not understood! \n\r";
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
	if(strcmp(input, "quit") == 0) {

		// Say bye :)
		cout << "Bye!";
		Sleep(1000);

		// Exit program
		exit(0);
	}

	// If the user wants to list local resources
	else if(strcmp(input, "list") == 0) {

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
		
		return true;
	}

	// If the input is not understood
	else
		return false;

}