#include "PlayState.h"
#include "InputHandler.h"
#include "Game.h"

const std::string PlayState::s_playID = "PLAY";

void PlayState::update() {

}

void PlayState::render() {

}

bool PlayState::onEnter() {
  TheGame::Instance()->setPlayerLives(3);

  std::cout << "entering PlayState\n";
  return true;
}

bool PlayState::onExit() {
  m_exiting = true;
  TheInputHandler::Instance()->reset();
  std::cout << "exiting PlayState\n";
  return true;
}