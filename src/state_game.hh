#ifndef STATE_GAME_HH
#define STATE_GAME_HH

#include "state.hh"

class StateGame : public BaseState
{
public:
  void Load();
  void Draw();
};

extern StateGame state_game;

#endif

// vim: et:sw=2

