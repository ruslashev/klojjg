#include "state.hh"
#include "state_mainmenu.hh"
#include "state_game.hh"

StateDispatcher::StateDispatcher() {
  state_mainmenu.sd = this;
  state_game.sd = this;
  currentState = nullptr;
}

void StateDispatcher::ChangeTo(BaseState *newState) {
  if (currentState)
    currentState->Unload();
  currentState = newState;
  currentState->Load();
}

// vim: et:sw=2

