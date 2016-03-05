#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include "gfx/gui.hh"
#include "state.hh"

#include <GLFW/glfw3.h>
#include <cstdarg>

const struct
{
  int updateMilliseconds = 16;
  int antialiasing = 6;
  int fontMemoryGuess = 5*1024*1024;
  int fontSize = 20;
  int fontOversample = 8;
  float block_scale = 2;
} Constants {};

struct GlobalsHolder
{
  bool quit = false;

  int windowWidth = 1008;
  int windowHeight = 567;

  double mouse_x = 0, mouse_y = 0, mouse_accum_dx = 0, mouse_accum_dy = 0;
  bool mousePressed[3] = { 0, 0, 0 };

  // retarded a bit
  Gui *guiptr;
  StateDispatcher *stateDispatcherPtr;
  GLFWwindow *glfwWindowPtr;
};

extern GlobalsHolder Globals;

void die(const char *format, ...);
void warn(const char *format, ...);

#endif

// vim: et:sw=2

