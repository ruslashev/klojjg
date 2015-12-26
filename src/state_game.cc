#include "state_game.hh"
#include "state_mainmenu.hh"

#include <GL/glew.h>

StateGame state_game;

void StateGame::Draw()
{
  sd->ChangeTo(&state_mainmenu);
  glClearColor(255, 255, 255, 255);
  glClear(GL_COLOR_BUFFER_BIT);
}

// vim: et:sw=2

