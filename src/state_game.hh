#ifndef STATE_GAME_HH
#define STATE_GAME_HH

#include "state.hh"
#include "gfx/gl.hh"

class StateGame : public BaseState
{
  program *prog;
  GLint attribute_coord2d;
public:
  ~StateGame();
  void Load();
  void Draw();
};

extern StateGame state_game;

#endif

// vim: et:sw=2

