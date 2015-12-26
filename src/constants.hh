#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include "gfx/gui.hh"
#include "state.hh"
#include <cstdarg>

const struct
{
  int updateMilliseconds = 16;
  int antialiasing = 6;
  int fontMemoryGuess = 5*1024*1024;
  int fontSize = 20;
  int fontOversample = 8;
} Constants {};

struct GlobalsHolder
{
  bool quit = false;

  int windowWidth = 1008;
  int windowHeight = 567;

  double mousePosX, mousePosY;
  bool mousePressed[3];

  // retarded a bit
  Gui *guiptr;
  StateDispatcher *stateDispatcherPtr;
};

extern GlobalsHolder Globals;

void die(const char *format, ...);

#endif

// vim: et:sw=2

