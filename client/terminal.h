#ifndef TERMINAL_H
#define TERMINAL_H
#include <iostream>
#include <string>
#include <direct.h>
#include "../lib/dirent.h"
#include <sys/stat.h>
#include "../transfer.h"
#include <sstream>


class Terminal {
public:
	// functions
	Terminal();
	Terminal(SOCKET);
	void run();
	bool process(const char *);
	void quit();
	void listLocal();
	void showHelp();
	void listRemote();
	void putFile();

	// variables
	char directory[FILENAME_MAX];
	DIR *dir;
	struct dirent *dirEntry;
	Transfer transfer;
};

#endif