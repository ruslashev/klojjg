#include "state.hh"
#include "state_mainmenu.hh"
#include "state_game.hh"

StateDispatcher::StateDispatcher() {
  state_mainmenu.sd = this;
  state_game.sd = this;
}

void StateDispatcher::ChangeTo(BaseState *newState) {
  currentState = newState;
  currentState->Load();
}

// vim: et:sw=2

