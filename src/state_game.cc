#include "state_game.hh"

#include <GL/glew.h>

StateGame state_game;

void StateGame::Draw()
{
  glClearColor(255, 255, 255, 255);
  glClear(GL_COLOR_BUFFER_BIT);
}

// vim: et:sw=2

