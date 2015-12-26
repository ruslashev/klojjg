#ifndef STATE_HH
#define STATE_HH

class StateDispatcher;

class BaseState
{
public:
  StateDispatcher *sd;
  virtual ~BaseState() {};
  virtual void Load() {};
  virtual void Update(const double dt, const double time) {};
  virtual void Draw() {};
};

class StateDispatcher
{
public:
  BaseState *currentState;

  StateDispatcher();
  ~StateDispatcher();
  void ChangeTo(BaseState *newState);
};

#endif

// vim: et:sw=2

