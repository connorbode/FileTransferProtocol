#ifndef TERMINAL_H
#define TERMINAL_H
#include <iostream>
#include <windows.h>
#include <string>
#include <direct.h>
#include "../lib/dirent.h"
#include <sys/stat.h>
#include "../transfer.h"


class Terminal {
public:
	// functions
	Terminal(SOCKET);
	void run();
	bool process(const char *);
	void quit();
	void listLocal();
	void showHelp();

	// variables
	char directory[FILENAME_MAX];
	DIR *dir;
	struct dirent *dirEntry;
	Transfer transfer;
};

#endif