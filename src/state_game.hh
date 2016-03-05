#pragma once

#include "gfx/gl.hh"
#include "player.hh"
#include "state.hh"
#include "world.hh"

class StateGame : public BaseState
{
  Player ply;

  program *prog;
  GLint attribute_coord3d, attribute_texcoord,
        uniform_mvp, uniform_texid;
  buffer *vbo, *ebo, *tbo;
  GLuint textureid;
  int elements_n;
  world *mainworld;
public:
  ~StateGame();
  void Load();
  void Unload();
  void Draw();
  void Update(const double dt, const double time);
  void onKeyInput(int key, int scancode, int action, int mods);
};

extern StateGame state_game;

// vim: et:sw=2

