#include "PlayState.h"
#include "GameOverState.h"
#include "PauseState.h"
#include "Game.h"
#include "InputHandler.h"
#include "LevelParser.h"
#include "Level.h"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter() {
  // parse the Level
  LevelParser levelParser;
  pLevel = levelParser.parseLevel("assets/map1.tmx");

  std::cout << "entering PlayState\n";
  return true;
}


void PlayState::update() {
  // Pause when press Escape
  if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE)) {
    TheGame::Instance()->getStateMachine()->pushState(new PauseState());
  }
  if (pLevel != NULL)
    pLevel->update();
}

void PlayState::render() {
  pLevel->render();
}



bool PlayState::onExit() {
  for (int i=0; i<m_gameObjects.size(); i++)
    m_gameObjects[i]->clean();
  m_gameObjects.clear();
  // clear the TextureManager
  for (int i=0; i<m_textureIDList.size(); i++)
    TheTextureManager::Instance()->clearFromTextureMap(m_textureIDList[i]);
  std::cout << "exiting PlayState\n";
  return true;
}

bool PlayState::checkCollision(SDLGameObject* p1, SDLGameObject* p2) {
  int leftA, leftB; // x1 of each object
  int rightA, rightB; // x2 of each object
  int topA, topB; // y1 of each object 
  int bottomA, bottomB; // y2 of each object

  leftA = p1->getPosition().getX(); // x1 of object A
  rightA = p1->getPosition().getX() + p1->getWidth(); // x2 of object A
  topA = p1->getPosition().getY(); // y1 of object A
  bottomA = p1->getPosition().getY() + p1->getHeight(); // y2 of object A

  leftB = p2->getPosition().getX(); // x1 of object A
  rightB = p2->getPosition().getX() + p2->getWidth(); // x2 of object B
  topB = p2->getPosition().getY(); // y1 of object B
  bottomB = p2->getPosition().getY() + p2->getHeight(); // y2 of object B

  // if any side of A is outside B -> no collision
  if (bottomA <= topB) { return false; }
  if (topA >= bottomB) { return false; }
  if (rightA <= leftB) { return false; }
  if (leftA >= rightB) { return false; }
  
  return true;
}