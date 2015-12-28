#include "constants.hh"

GlobalsHolder Globals {};

void die(const char *format, ...)
{
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  printf("\n");
  exit(0);
}

void warn(const char *format, ...)
{
  printf("warning: ");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  printf("\n");
}

// vim: et:sw=2

