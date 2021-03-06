#include "constants.hh"
#include "state_game.hh"
#include "state_mainmenu.hh"
#include "tile.c"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../deps/tinyobjloader/tiny_obj_loader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

StateGame state_game;

StateGame::~StateGame()
{
  glDeleteTextures(1, &textureid);
  delete prog;
}

void StateGame::Load()
{
  glfwSetInputMode(Globals.glfwWindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  ply.position = glm::vec3(0, 0, 0);
  ply.view_angles = glm::vec2(0, 0);
  ply.computeViewMatrix();

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
      positions.size() * sizeof(GLfloat), positions.data());
  ebo = new buffer(GL_ELEMENT_ARRAY_BUFFER,
      elements.size() * sizeof(GLushort), elements.data());
  tbo = new buffer(GL_ARRAY_BUFFER,
      texcoords.size() * sizeof(GLfloat), texcoords.data());

  glEnable(GL_DEPTH_TEST);

  glGenTextures(1, &textureid);
  glBindTexture(GL_TEXTURE_2D, textureid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tile.width, tile.height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, tile.pixel_data);
}

void StateGame::Unload()
{
  glfwSetInputMode(Globals.glfwWindowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void StateGame::Update(const double dt, const double time)
{
  const float speed = 10;
  const float dtf = dt / 1000.0f;
  if (glfwGetKey(Globals.glfwWindowPtr, GLFW_KEY_W) == GLFW_PRESS)
    ply.position += ply.view_direction * speed * dtf;
  if (glfwGetKey(Globals.glfwWindowPtr, GLFW_KEY_S) == GLFW_PRESS)
    ply.position -= ply.view_direction * speed * dtf;
  if (glfwGetKey(Globals.glfwWindowPtr, GLFW_KEY_A) == GLFW_PRESS)
    ply.position -= ply.view_right * speed * dtf;
  if (glfwGetKey(Globals.glfwWindowPtr, GLFW_KEY_D) == GLFW_PRESS)
    ply.position += ply.view_right * speed * dtf;

  const double sensitivity = 2.2,
        m_pitch = 0.022, m_yaw = 0.022;

  ply.view_angles.x -= m_pitch * Globals.mouse_accum_dy * sensitivity;
  ply.view_angles.y -= m_yaw * Globals.mouse_accum_dx * sensitivity;
  Globals.mouse_accum_dx = Globals.mouse_accum_dy = 0;

  if (ply.view_angles.y > 360)
    ply.view_angles.y -= 360;
  if (ply.view_angles.y < -360)
    ply.view_angles.y -= -360;
  if (ply.view_angles.x > 89.999)
    ply.view_angles.x = 89.999;
  if (ply.view_angles.x < -89.999)
    ply.view_angles.x = -89.999;

  glm::mat4 view = ply.computeViewMatrix(),
    projection = glm::perspective(glm::radians(90.f),
        (float)Globals.windowWidth/(float)Globals.windowHeight, 0.1f, 100.0f),
    mvp = projection * view;

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

void StateGame::onKeyInput(int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    Globals.quit = true;
}

// vim: et:sw=2

