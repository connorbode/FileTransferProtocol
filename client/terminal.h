#ifndef TERMINAL_H
#define TERMINAL_H
#include <iostream>
#include <windows.h>

class Terminal {
public:
	Terminal();
	void run();
	bool process(const char *);
};

#endif