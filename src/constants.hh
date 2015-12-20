#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include "gui.hh"
#include <cstdarg>

const struct
{
	int updateMilliseconds = 16;
	int antialiasing = 6;
	int fontMemoryGuess = 5*1024*1024;
	int fontSize = 18;
} Constants {};

struct GlobalsHolder
{
	bool quit = false;

	int windowWidth = 800;
	int windowHeight = 450;

	// retarded
	Gui *guiptr;
};

extern GlobalsHolder Globals;

void die(const char *format, ...);

#endif

