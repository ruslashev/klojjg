#include "state.hh"
#include "state_mainmenu.hh"
#include "state_game.hh"

StateDispatcher::StateDispatcher() {
  // extern StateMainmenu state_mainmenu;
  // extern StateGame state_game;
  // state_mainmenu.sd = this;
  // state_game.sd = this;
}

StateDispatcher::~StateDispatcher() {
  // delete state_mainmenu;
  // delete state_game;
}

void StateDispatcher::ChangeTo(BaseState *newState) {
  currentState = newState;
  currentState->Load();
}

// vim: et:sw=2

