#ifndef STATE_HH
#define STATE_HH

class StateDispatcher;

class BaseState
{
public:
  StateDispatcher *sd;
  virtual ~BaseState() {};
  virtual void Load() {};
  virtual void Unload() {};
  virtual void Update(const double dt, const double time) {};
  virtual void Draw() {};
  virtual void onKeyInput(int key, int scancode, int action, int mods) {};
};

class StateDispatcher
{
public:
  BaseState *currentState;

  StateDispatcher();
  void ChangeTo(BaseState *newState);
};

#endif

// vim: et:sw=2

