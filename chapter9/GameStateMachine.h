#pragma once 
#include "GameState.h"

class GameStateMachine {
public:
  GameStateMachine() {}
  ~GameStateMachine() {}

  void update();
  void render();
  void clean();

  void pushState(GameState* pState);
  void changeState(GameState* pState);
  void popState();

  std::vector<GameState*>& getGameStates() { return m_gameStates; }

private:
  std::vector<GameState*> m_gameStates;  
};
// DONE