#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include "InputHandler.h"
#include "PauseState.h"
#include "Enemy.h"
#include "GameOverState.h"
#include "SDLGameObject.h"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

void PlayState::update() {
  // Pause when press Escape
  if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE)) {
    TheGame::Instance()->getStateMachine()->pushState(new PauseState());
  }
  // update GameObjects in PlayState
  for (int i=0; i<m_gameObjects.size(); i++) 
    m_gameObjects[i]->update();

  if (checkCollision(dynamic_cast<SDLGameObject*>(m_gameObjects[0]), dynamic_cast<SDLGameObject*>(m_gameObjects[1])))
    TheGame::Instance()->getStateMachine()->pushState(new GameOverState());
}

void PlayState:: render() {
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->draw();
  }
}

bool PlayState::onEnter() {
  if (!TheTextureManager::Instance()->load("assets/helicopter.png", "helicopter", TheGame::Instance()->getRenderer()))
    return false;
  if (!TheTextureManager::Instance()->load("assets/helicopter2.png", "helicopter2", TheGame::Instance()->getRenderer()))
    return false;
  GameObject* player = new Player(new LoaderParams(500, 100, 128, 55, "helicopter"));
  GameObject* enemy = new Enemy(new LoaderParams(100, 100, 128, 55, "helicopter2"), 5);
  m_gameObjects.push_back(player);
  m_gameObjects.push_back(enemy);
  std::cout << "entering PlayState\n";
  return true;
}

bool PlayState::onExit() {
  for (int i=0; i<m_gameObjects.size(); i++)
    m_gameObjects[i]->clean();
  m_gameObjects.clear();
  TheTextureManager::Instance()->clearFromTextureMap("helicopter");
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