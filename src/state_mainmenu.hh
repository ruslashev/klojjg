#pragma once

#include "state.hh"

class StateMainmenu : public BaseState
{
public:
  void Draw();
  void onKeyInput(int key, int scancode, int action, int mods);
};

extern StateMainmenu state_mainmenu;

// vim: et:sw=2

