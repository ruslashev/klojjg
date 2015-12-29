#include "state_game.hh"
#include "state_mainmenu.hh"

#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

StateGame state_game;

StateGame::~StateGame()
{
  delete prog;
}

void StateGame::Load()
{
  prog = new program("content/main.vs", "content/main.fs");
  attribute_coord3d = prog->bind_attribute("coord3d");
  uniform_mat_transform = prog->bind_uniform("mat_transform");

  GLfloat verts[] = {
     0.0,  0.8, 0.0,
    -0.8, -0.8, 0.0,
     0.8, -0.8, 0.0
  };
  vbo = new buffer(GL_ARRAY_BUFFER, sizeof(verts), verts);
}

void StateGame::Update(const double dt, const double time)
{
  float angle = (time / 1000.0) * 45.0,
        move = sinf(2 * M_PI * (time / 1000.0) / 5);
  const glm::vec3 axis_z = glm::vec3(0, 0, 1);
  const glm::mat4 id = glm::mat4(1.0f);
  glm::mat4 rotate = glm::rotate(id, glm::radians(angle), axis_z);
  glm::mat4 translate = glm::translate(id, glm::vec3(move, 0.0, 0.0));
  glm::mat4 transform = translate*rotate;

  glUniformMatrix4fv(uniform_mat_transform, 1, GL_FALSE, glm::value_ptr(transform));
}

void StateGame::Draw()
{
  glClearColor(255, 255, 255, 255);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(prog->id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  glEnableVertexAttribArray(attribute_coord3d);
  glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(attribute_coord3d);
}

// vim: et:sw=2

