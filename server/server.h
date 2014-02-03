#ifndef SERVER_H
#define SERVER_H

#include "../transfer.h"
#include <iostream>
#include "../lib/dirent.h"
#include <sys/stat.h>
#include <direct.h>
#include <vector>
#include <conio.h>
#include <sstream>
#include <string>

class Server {

public:
	// methods
	Server();
	Server(SOCKET);
	void run();
	void list();
	void put(std::string);

	// variables
	Transfer transfer;
	char directory[FILENAME_MAX];
	DIR *dir;
	struct dirent *dirEntry;
};

#endif
