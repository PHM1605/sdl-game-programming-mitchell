#include "GameStateMachine.h"

void GameStateMachine::pushState(GameState* pState) {
  m_gameStates.push_back(pState);
  m_gameStates.back()->onEnter();
}

void GameStateMachine::popState() {
  if (!m_gameStates.empty()) {
    // calling the onExit() function on the last State
    if (m_gameStates.back()->onExit()) {
      // delete m_gameStates.back(); // delete what last State-pointer points to
      m_gameStates.pop_back(); // delete the last Pointer
    }
  }
}

void GameStateMachine::changeState(GameState* pState) {
  // delete the last State if exists AND pushing State is different
  if (!m_gameStates.empty()) {
    if (m_gameStates.back()->getStateID() == pState->getStateID())
      return; // do nothing
    m_gameStates.back()->onExit();
    //delete m_gameStates.back();
    m_gameStates.pop_back();
  }
  pState->onEnter();
  // push back our new State
  m_gameStates.push_back(pState);
}

void GameStateMachine::update() {
  if (!m_gameStates.empty()) {
    m_gameStates.back()->update();
  }
}

#include <iostream>
void GameStateMachine::render() {
  if (!m_gameStates.empty()) {
    m_gameStates.back()->render();
  }
}