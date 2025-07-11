#include "PlayState.h"
#include "GameOverState.h"
#include "PauseState.h"
#include "Game.h"
#include "InputHandler.h"
#include "LevelParser.h"
#include "Level.h"
#include "BulletHandler.h"
#include "TextureManager.h"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter() {
  TheGame::Instance()->setPlayerLives(3);
  // parse the Level
  LevelParser levelParser;
  pLevel = levelParser.parseLevel(TheGame::Instance()->getLevelFiles()[TheGame::Instance()->getCurrentLevel()-1].c_str());
  bool tmp;
  TheTextureManager::Instance()->load("assets/bullet1.png", "bullet1", TheGame::Instance()->getRenderer());
  TheTextureManager::Instance()->load("assets/bullet2.png", "bullet2", TheGame::Instance()->getRenderer());
  TheTextureManager::Instance()->load("assets/bullet3.png", "bullet3", TheGame::Instance()->getRenderer());
  TheTextureManager::Instance()->load("assets/lives.png", "lives", TheGame::Instance()->getRenderer());

  if (pLevel != 0) 
    m_loadingComplete = true;
  std::cout << "entering PlayState\n";
  return true;
}

void PlayState::update() {
  if (m_loadingComplete && !m_exiting) {
    // Pause when press Escape
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE)) {
      TheGame::Instance()->getStateMachine()->pushState(new PauseState());
    }
    TheBulletHandler::Instance()->updateBullets();
    if (TheGame::Instance()->getPlayerLives() == 0) {
      TheGame::Instance()->getStateMachine()->changeState(new GameOverState());
    }
  }
  
  if (pLevel != NULL)
    pLevel->update();
}

void PlayState::render() {
  if (m_loadingComplete) {
    if (pLevel != 0) pLevel->render();
    for (int i=0; i<TheGame::Instance()->getPlayerLives(); i++)
    {
      TheTextureManager::Instance()->drawFrame("lives", i*30, 0, 32, 30, 0, 0, TheGame::Instance()->getRenderer(), 0.0, 255);
    }
    TheBulletHandler::Instance()->drawBullets();
  }
}

bool PlayState::onExit() {
  m_exiting = true;
  TheInputHandler::Instance()->reset();
  TheBulletHandler::Instance()->clearBullets();
  
  std::cout << "exiting PlayState\n";
  return true;
}
