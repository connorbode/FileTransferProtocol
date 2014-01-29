#include "terminal.h"

using namespace std;

Terminal::Terminal() {

}

/**
 * Starts the Terminal running & waiting for input
 */
void Terminal::run() {

	// Wait for commands until quit
	while(true) {

		// Wait for input
		cout << "waiting for input: ";
		char input[128] = "";
		cin >> input;
		cout << "\n\r";

		// Process input
		if( ! process(input) ) {
			cout << "input not understood!";
			cout << "\n\r";
		}
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
		std::exit(0);
	}

	// If the input is not understood
	else
		return false;

}