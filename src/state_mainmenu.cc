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

// vim: et:sw=2

