#ifndef STATE_GAME_HH
#define STATE_GAME_HH

#include "gfx/gl.hh"
#include "state.hh"
#include "world.hh"

class StateGame : public BaseState
{
  program *prog;
  GLint attribute_coord3d,
        uniform_mvp;
  buffer *vbo, *ebo;
  int elements_n;
  world *mainworld;
public:
  ~StateGame();
  void Load();
  void Draw();
  void Update(const double dt, const double time);
};

extern StateGame state_game;

#endif

// vim: et:sw=2

