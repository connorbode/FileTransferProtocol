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

	transfer.sendMessage("uiVKRfyr6OSOPPzCfQ9WXFSB6c2zwlvpXImA0TiSNrbuBLXzUGvKkxV1hIa0GxcfRUOOyfOgiYSWY0fVIS3mrwL7IMqg8Lrf3yCa3igO51Q5tVhTKpaIpujoEFBXn7lngfqu1vg1iYPSQg2SuERdowtoA1qAEhpoqq3KiHK5AZs3zSn1p9xkQCU2PfaTHhrASbDrtNfyBnLwrtoKMUTcBQVCoEZWIOAw0aPZfKF94Q8HCWpzz6aIwJRpC361iz0aEGlhmvNFrj0NfLDQZUl7OuiEEaqV4KYbxLSOLhEm49JO8DZ0mD4MA4zazTtzY2hST0zy4nrYKAB8DLRGtS85zHajlqr5kYadbph0EtYAnSaq2ES8aHeIDiVHfl3kSkBlzNaWsweu6DnhgMIv3VxZjbaHhlgtxDKb7ROSonQZjzSNUdurEZW0n1SfiDKlBMC2k7ElkBEo2ACvfUeCfjQrWeX0HjCc5EiPTtC8rqV6bXE8DI8J3SMYKlEdSSnCEH24H6r0l87LjACroO0pQHHxjFESk0k0h8e6gQIRCvcG6srM0iNZKl5LN3OByBGov9Uuqg7G3YE4LVHy7mX0JxHuvLk6WXW5tnqh8381sF99CnVxjfFpuxkOdm5iFe3Eml7gQUZ7f0NQxwJkoOzHxDcrptmRvjEN0Y27WX4YUe5ZeDSZIkrpCnizJIrtSHs7fxIYIm8H6fMq8H7IXvM8RwDoPrX75NsmcUh4uIfeKiv5lcLIT1W4TTEW35tE0YPejll4w7M4Qy3AsV9s5x2CM3KD8ItAYVUj6YhiZ5dfLzm5TU25M8lgKxNCL5hehc01t5MBOcAyF52mS0YgeXczQxS93eBv87Jy0Ji2QxtOnblkFkMflTkcCw4aKZe1qoh2JCEaC5pdR9KH6RP135yDSTcixMIR4wcJu3TddKUxQXeLShq3v8GEdR2wl6Tzjrx9lIXpVfWCxu24M948kt6yHxNQNokxu7kO4d6kyD8BGrWUKfxyUp0zf2QBqF22L4ItYhHRCIqaqoDBcQ45Bd5RNPi9tOlPhf2nYtOQc03YFg4sdgPJIB20TBXvnZ1FyhcW5knmVS8Em3gv0xV4XtmmZucjfxKdZ6Ye6BW0mGzaEFIy5KuQKzfrpPDDrG44zjs3kZOe7Z0AME7upGq4aZNDa9tM1LqCuMd02qRdEQozYP3VQAYRipi9YAjWu0ARxxtQNPcB5fcR0UBR0TfxzUHEWnx5hphcJIC3EJYVxf6tLmahtIyLyMgVHYsmXIhucXYJqLLkEde2dXGP41bFx6PLPINdjFd42qwbPCygOjKx31TECGCu4M3YNweTy05ajpTyj71KzUxjtw9tNOlYcv5CBMpNqcrBruk7jUN3imMlz92T3eiQ24z5NY51qDYc");
	
	getch();

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