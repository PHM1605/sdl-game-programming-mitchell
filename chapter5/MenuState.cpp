#include <iostream>
#include "MenuState.h"
#include "GameState.h"
#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"

const std::string MenuState::s_menuID = "MENU";

void MenuState::update() {  
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->update();
  }
}

void MenuState::render() {
  for (int i=0; i<m_gameObjects.size(); ++i) {
    m_gameObjects[i]->draw();
  }
}

bool MenuState::onEnter() {
  if (!TheTextureManager::Instance()->load("assets/button.png", "playbutton", TheGame::Instance()->getRenderer())) {
    return false;
  }
  if (!TheTextureManager::Instance()->load("assets/exit.png", "exitbutton", TheGame::Instance()->getRenderer())) {
    return false;
  }
  // Buttons
  GameObject* button1 = new MenuButton(new LoaderParams(100, 100, 400, 100, "playbutton"));
  GameObject* button2 = new MenuButton(new LoaderParams(100, 300, 400, 100, "exitbutton"));
  m_gameObjects.push_back(button1);
  m_gameObjects.push_back(button2);
  std::cout << "entering MenuState\n";

  return true;
}

bool MenuState::onExit() {
  // clear what pointers pointing to
  for (int i=0; i<m_gameObjects.size(); i++) 
    m_gameObjects[i]->clean();
  // clear the pointers' container itself
  m_gameObjects.clear();
  // Clear TextureManager
  TheTextureManager::Instance()->clearFromTextureMap("playbutton");
  TheTextureManager::Instance()->clearFromTextureMap("exitbutton");
  
  std::cout << "exiting MenuState\n";
  return true;
}