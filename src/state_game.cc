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

#include "tile.c"

StateGame state_game;

StateGame::~StateGame()
{
  glDeleteTextures(1, &textureid);
  delete prog;
}

void StateGame::Load()
{
  prog = new program("content/main.vs", "content/main.fs");
  attribute_coord3d = prog->bind_attribute("coord3d");
  attribute_texcoord = prog->bind_attribute("texcoord");
  uniform_mvp = prog->bind_uniform("mvp");
  uniform_texid = prog->bind_uniform("texid");

  std::vector<GLfloat> positions, normals, texcoords;
  std::vector<GLushort> elements;

  mainworld = new world;
  mainworld->load_from_file("content/levelone.vxl");
  mainworld->generate_mesh(positions, elements, texcoords);

  elements_n = elements.size();
  printf("tris: %d\n", elements_n/3);

  vbo = new buffer(GL_ARRAY_BUFFER,
      positions.size()*sizeof(GLfloat), positions.data());
  ebo = new buffer(GL_ELEMENT_ARRAY_BUFFER,
      elements.size()*sizeof(GLushort), elements.data());
  tbo = new buffer(GL_ARRAY_BUFFER,
      texcoords.size()*sizeof(GLfloat), texcoords.data());

  glEnable(GL_DEPTH_TEST);

  glGenTextures(1, &textureid);
  glBindTexture(GL_TEXTURE_2D, textureid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tile.width, tile.height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, tile.pixel_data);
}

void StateGame::Update(const double dt, const double time)
{
  float angle = (time / 1000.0) * 45.0,
        move = sin(2.0 * M_PI * (time / 1000.0) / 5.0);
  const glm::vec3 axis_z = glm::vec3(0, 0, 1),
        axis_y = glm::vec3(0, 1, 0),
        axis_x = glm::vec3(1, 0, 0);
  const glm::mat4 id = glm::mat4(1.0f);

  glm::mat4
    rotate = glm::rotate(id, glm::radians(angle), axis_y),
           translate = glm::translate(id, glm::vec3(0, move, 0)),
           model = translate * rotate,
           view = glm::lookAt(glm::vec3(0.0, 1.0, -2.0), glm::vec3(0, 0, 0), axis_y),
           projection = glm::perspective(73.74f,
               (float)Globals.windowWidth/(float)Globals.windowHeight, 0.1f, 100.0f),
           mvp = projection * view * model;

  glUseProgram(prog->id);
  glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
}

void StateGame::Draw()
{
  glClearColor(255, 255, 255, 255);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(prog->id);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureid);
  glUniform1i(uniform_texid, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
  glEnableVertexAttribArray(attribute_coord3d);
  glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->id);

  glBindBuffer(GL_ARRAY_BUFFER, tbo->id);
  glEnableVertexAttribArray(attribute_texcoord);
  glVertexAttribPointer(attribute_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawElements(GL_TRIANGLES, elements_n, GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(attribute_coord3d);
  glDisableVertexAttribArray(attribute_texcoord);
}

// vim: et:sw=2

