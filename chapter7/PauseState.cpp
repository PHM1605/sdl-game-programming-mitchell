#include "PauseState.h"
#include "MainMenuState.h"
#include "Game.h"
#include "MenuButton.h"
#include "InputHandler.h"
#include "StateParser.h"
#include "TextureManager.h"

const std::string PauseState::s_pauseID = "PAUSE";

void PauseState::s_pauseToMain() {
  TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

void PauseState::s_resumePlay() {
  TheGame::Instance()->getStateMachine()->popState();
}

void PauseState::update() {
  for (int i=0; i<m_gameObjects.size(); i++)
    m_gameObjects[i]->update();
}

void PauseState::render() {
  for (int i=0; i<m_gameObjects.size(); i++)
    m_gameObjects[i]->draw();
}

bool PauseState::onEnter() {
  StateParser stateParser;
  stateParser.parseState("assets/attack.xml", s_pauseID, &m_gameObjects, &m_textureIDList);
  // set the callbacks; then assign them to the corresponding Buttons
  m_callbacks.push_back(0);
  m_callbacks.push_back(s_pauseToMain);
  m_callbacks.push_back(s_resumePlay);
  setCallbacks(m_callbacks);
  
  std::cout << "entering PauseState\n";
  return true;
}

bool PauseState::onExit() {
  // clear GameObjects
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->clean();
  }
  m_gameObjects.clear();
  // clear the TextureManager
  for (int i=0; i<m_textureIDList.size(); i++)
    TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
  
  std::cout << "exit PauseState\n";
  return true;
}

void PauseState::setCallbacks(const std::vector<Callback>& callbacks) {
  for (int i=0; i<m_gameObjects.size(); i++) {
    // assign callbacks only when they are MenuButtons
    if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
      MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
      pButton->setCallback(callbacks[pButton->getCallbackID()]);
    }
  }
}