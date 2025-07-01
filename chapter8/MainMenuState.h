#pragma once
#include "GameState.h"
#include "GameObject.h"
#include "MenuState.h"
#include <vector>

class MainMenuState: public MenuState {
public: 
  virtual ~MainMenuState() {}
  virtual void update();
  virtual void render();
  virtual bool onEnter();
  virtual bool onExit();
  virtual std::string getStateID() const { return s_menuID; }
  
private:
  virtual void setCallbacks(const std::vector<Callback>& callbacks);
  // callbacks when click "Play" or "Exit" buttons
  static void s_menuToPlay();
  static void s_exitFromMenu();
  static const std::string s_menuID;
  std::vector<GameObject*> m_gameObjects;
};