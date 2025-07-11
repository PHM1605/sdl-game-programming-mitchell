#include "GameOverState.h"
#include "InputHandler.h"
#include "PauseState.h"
#include "Game.h"
#include "MainMenuState.h"
#include "PlayState.h"
#include "AnimatedGraphic.h"
#include "MenuButton.h"
#include "StateParser.h"
const std::string GameOverState::s_gameOverID = "GAMEOVER";

void GameOverState::update() {
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->update();
  }
}

void GameOverState::render() {
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->draw();
  }
}

void GameOverState::s_gameOverToMain() {
  TheGame::Instance()->getStateMachine()->changeState(new MainMenuState());
}

void GameOverState::s_restartPlay() {
  TheGame::Instance()->getStateMachine()->changeState(new PlayState());
}

bool GameOverState::onEnter() {
  // parse the State
  StateParser stateParser;
  stateParser.parseState("test.xml", s_gameOverID, &m_gameObjects, &m_textureIDList);
  // setup callbacks for Buttons in State
  m_callbacks.push_back(0);
  m_callbacks.push_back(s_gameOverToMain);
  m_callbacks.push_back(s_restartPlay);
  setCallbacks(m_callbacks);

  std::cout << "entering PauseState\n";
  return true;
}

bool GameOverState::onExit() {
  // clear GameObjects in State
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->clean();
  }
  m_gameObjects.clear();
  // clear the TextureManager
  for (int i=0; i<m_textureIDList.size(); i++) {
    TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
  }

  std::cout << "exiting GameOverState" << std::endl;
  return true;
}

void GameOverState::setCallbacks(const std::vector<Callback>& callbacks) {
  for (int i=0; i<m_gameObjects.size(); i++) {
    if (dynamic_cast<MenuButton*>(m_gameObjects[i])) {
      MenuButton* pButton = dynamic_cast<MenuButton*>(m_gameObjects[i]);
      pButton->setCallback(callbacks[pButton->getCallbackID()]);
    }      
  }
}