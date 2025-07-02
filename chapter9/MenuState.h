#pragma once 
#include "GameState.h"

// to add callbacks functionality to normal GameState
class MenuState: public GameState {
public:
  virtual ~MenuState() {}

protected:
  typedef void(*Callback)(); 
  virtual void setCallbacks(const std::vector<Callback>& callbacks) = 0;
  std::vector<Callback> m_callbacks;
};