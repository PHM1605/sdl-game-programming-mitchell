#include <iostream>
#include "MainMenuState.h"
#include "GameState.h"
#include "TextureManager.h"
#include "Game.h"
#include "MenuButton.h"
#include "PlayState.h"
#include "StateParser.h"

const std::string MainMenuState::s_menuID = "MENU";

void MainMenuState::update() {  
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->update();
  }
}

void MainMenuState::render() {
  for (int i=0; i<m_gameObjects.size(); ++i) {
    m_gameObjects[i]->draw();
  }
}

bool MainMenuState::onEnter() {
  std::cout << "entering MenuState\n";
  // parse the State
  StateParser stateParser;
  stateParser.parseState("test.xml", s_menuID, &m_gameObjects, &m_textureIDList);
  m_callbacks.push_back(nullptr); // callbackID starts from 1
  m_callbacks.push_back(s_menuToPlay);
  m_callbacks.push_back(s_exitFromMenu);
  
  setCallbacks(m_callbacks);
  return true;
}

void MainMenuState::setCallbacks(const std::vector<Callback>& callbacks) {
  for (int i=0; i<m_gameObjects.size(); i++) {
    if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
      MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
      pButton->setCallback(callbacks[pButton->getCallbackID()]);
    }
  }
}

bool MainMenuState::onExit() {
  // clear what pointers pointing to
  for (int i=0; i<m_gameObjects.size(); i++) 
    m_gameObjects[i]->clean();
  // clear the pointers' container itself
  m_gameObjects.clear();
  // Clear TextureManager
  for (int i=0; i<m_textureIDList.size(); i++) 
    TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
  
  std::cout << "exiting MenuState\n";
  return true;
}

void MainMenuState::s_menuToPlay() {
  TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

void MainMenuState::s_exitFromMenu() {
  TheGame::Instance()->quit(); // set m_bRunning = false
}