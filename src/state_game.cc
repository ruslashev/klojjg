#include "constants.hh"
#include "state_game.hh"
#include "state_mainmenu.hh"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tinyobjloader/tiny_obj_loader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

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

  std::vector<GLfloat> positions, normals;
  std::vector<GLushort> elements;

  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err;
  bool ret = tinyobj::LoadObj(shapes, materials, err, "content/level.obj");
  if (!err.empty())
    warn("%s", err.c_str());
  if (!ret)
    die("error at parsing \"content/level.obj\"");

  for (size_t i = 0; i < shapes.size(); i++) {
    for (size_t f = 0; f < shapes[i].mesh.indices.size(); f++)
      elements.push_back(shapes[i].mesh.indices[f]);
    for (size_t v = 0; v < shapes[i].mesh.positions.size(); v++)
      positions.push_back(shapes[i].mesh.positions[v]);
  }
  elements_n = elements.size();

  vbo = new buffer(GL_ARRAY_BUFFER,
      positions.size()*sizeof(GLfloat), positions.data());
  ebo = new buffer(GL_ELEMENT_ARRAY_BUFFER,
      elements.size()*sizeof(GLushort), elements.data());

  glEnable(GL_DEPTH_TEST);
}

void StateGame::Update(const double dt, const double time)
{
  float angle = (time / 1000.0) * 45.0,
        move = sin(2.0 * M_PI * (time / 1000.0) / 5.0);
  const glm::vec3 axis_z = glm::vec3(0, 0, 1),
        axis_y = glm::vec3(0, 1, 0);
  const glm::mat4 id = glm::mat4(1.0f);

  glm::mat4
    rotate = glm::rotate(id, glm::radians(angle), axis_y),
           translate = glm::translate(id, glm::vec3(0, move, 0)),
           model = translate * rotate,
           view = glm::lookAt(glm::vec3(0.0, 1.0, -2.0), glm::vec3(0, 0, 0), axis_y),
           projection = glm::perspective(90.0f,
               (float)Globals.windowWidth/(float)Globals.windowHeight, 0.1f, 1000.0f),
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

  glDrawElements(GL_TRIANGLES, elements_n, GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(attribute_coord3d);
}

// vim: et:sw=2

