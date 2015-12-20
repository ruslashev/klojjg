#include "constants.hh"

GlobalsHolder Globals {};

void die(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\n");
	exit(0);
}

