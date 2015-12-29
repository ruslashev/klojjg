#include "constants.hh"
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
  uniform_mvp = prog->bind_uniform("mvp");

  GLfloat verts[] = {
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
  };
  vbo = new buffer(GL_ARRAY_BUFFER, sizeof(verts), verts);
  GLushort elements[] = {
    0, 1, 2, // front
    2, 3, 0,
    1, 5, 6, // top
    6, 2, 1,
    7, 6, 5, // back
    5, 4, 7,
    4, 0, 3, // bottom
    3, 7, 4,
    4, 5, 1, // left
    1, 0, 4,
    3, 2, 6, // right
    6, 7, 3,
  };
  ebo = new buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements);

  glEnable(GL_DEPTH_TEST);
}

void StateGame::Update(const double dt, const double time)
{
  float angle = (time / 1000.0) * 45.0,
        move = sinf(2 * M_PI * (time / 1000.0) / 5);
  const glm::vec3 axis_z = glm::vec3(0, 0, 1),
        axis_y = glm::vec3(0, 1, 0);
  const glm::mat4 id = glm::mat4(1.0f);

  glm::mat4 model = id,
    view = glm::lookAt(glm::vec3(0.0, 2.0, 4.0), glm::vec3(0, 0, 0), axis_y),
    projection = glm::perspective(90.0f,
        (float)Globals.windowWidth/(float)Globals.windowHeight, 0.1f, 10.0f),
    mvp = projection * view * model;

  glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void StateGame::Draw()
{
  glClearColor(255, 255, 255, 255);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(prog->id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  glEnableVertexAttribArray(attribute_coord3d);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);
  glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

  int size;
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(attribute_coord3d);
}

// vim: et:sw=2

