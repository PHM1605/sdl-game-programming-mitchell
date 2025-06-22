#pragma once
#include "GameState.h"
#include "GameObject.h"
#include <vector>

class MenuState: public GameState {
public: 
  virtual void update();
  virtual void render();
  virtual bool onEnter();
  virtual bool onExit();
  virtual std::string getStateID() const { return s_menuID; }
  
private:
  static const std::string s_menuID;
  std::vector<GameObject*> m_gameObjects;
  // callbacks when click "Play" or "Exit" buttons
  static void s_menuToPlay();
  static void s_exitFromMenu();
};