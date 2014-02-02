#ifndef SERVER_H
#define SERVER_H

#include "../transfer.h"
#include <iostream>
#include "../lib/dirent.h"
#include <sys/stat.h>
#include <direct.h>
#include <vector>
#include <conio.h>

class Server {

public:
	// methods
	Server();
	Server(SOCKET);
	void run();
	void list();

	// variables
	Transfer transfer;
	char directory[FILENAME_MAX];
	DIR *dir;
	struct dirent *dirEntry;
};

#endif
