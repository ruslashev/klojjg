#include "state_game.hh"
#include "state_mainmenu.hh"

#include <GL/glew.h>

StateGame state_game;

StateGame::~StateGame()
{
  delete prog;
}

void StateGame::Load()
{
  prog = new program("content/main.vs", "content/main.fs");
  attribute_coord2d = prog->bind_attribute("coord2d");
}

void StateGame::Draw()
{
  glClearColor(255, 255, 255, 255);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(prog->id);
  glEnableVertexAttribArray(attribute_coord2d);
  GLfloat verts[] = {
    0.0,  0.8,
    -0.8, -0.8,
    0.8, -0.8,
  };
  glVertexAttribPointer( attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, verts);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(attribute_coord2d);
}

// vim: et:sw=2

