#include "PlayState.h"
#include "InputHandler.h"
#include "Game.h"
#include "LevelParser.h"
#include "TextureManager.h"

const std::string PlayState::s_playID = "PLAY";

void PlayState::update() {

}

void PlayState::render() {
  if (m_loadingComplete) {
    if (pLevel != 0) {
      pLevel->render();
    }
  }
}

bool PlayState::onEnter() {
  TheGame::Instance()->setPlayerLives(3);
  LevelParser levelParser;
  pLevel = levelParser.parseLevel(TheGame::Instance()->getLevelFiles()[TheGame::Instance()->getCurrentLevel()-1].c_str());
  if (pLevel != 0)
    m_loadingComplete = true;

  std::cout << "entering PlayState\n";
  return true;
}

bool PlayState::onExit() {
  m_exiting = true;
  TheInputHandler::Instance()->reset();
  std::cout << "exiting PlayState\n";
  return true;
}