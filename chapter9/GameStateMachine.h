#pragma once 
#include "GameState.h"

class GameStateMachine {
public:
  GameStateMachine() {}
  ~GameStateMachine() {}

  void update();
  void render();

  void pushState(GameState* pState);
};