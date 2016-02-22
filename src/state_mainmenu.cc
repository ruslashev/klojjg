#include "state_mainmenu.hh"

#include "constants.hh"
#include "gfx/gui.hh"

#include <GL/glew.h>

StateMainmenu state_mainmenu;

void StateMainmenu::Draw()
{
  glClearColor(255, 255, 255, 255);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui::NewFrame();
  Globals.guiptr->Draw();
}

void StateMainmenu::onKeyInput(int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    Globals.quit = true;
  ImGuiIO& io = ImGui::GetIO();
  io.KeysDown[key] = (action != GLFW_RELEASE);
}

// vim: et:sw=2

