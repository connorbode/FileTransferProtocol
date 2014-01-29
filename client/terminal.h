#ifndef TERMINAL_H
#define TERMINAL_H
#include <iostream>
#include <windows.h>
#include <string>
#include <direct.h>
#include "../lib/dirent.h"
#include <sys/stat.h>


class Terminal {
public:
	// functions
	Terminal();
	void run();
	bool process(const char *);

	// variables
	char directory[FILENAME_MAX];
	DIR *dir;
	struct dirent *dirEntry;
};

#endif