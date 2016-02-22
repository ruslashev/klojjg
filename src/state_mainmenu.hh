#ifndef STATE_MAINMENU_HH
#define STATE_MAINMENU_HH

#include "state.hh"

class StateMainmenu : public BaseState
{
public:
  void Draw();
  void onKeyInput(int key, int scancode, int action, int mods);
};

extern StateMainmenu state_mainmenu;

#endif

// vim: et:sw=2

