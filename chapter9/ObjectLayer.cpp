#include "Camera.h"
#include "Game.h"
#include "Level.h"
#include "ObjectLayer.h"


ObjectLayer::~ObjectLayer() {
  for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it) {
    delete (*it);
  }
  m_gameObjects.clear();
}

void ObjectLayer::update(Level* pLevel) {
  // Player is dead if touch a Snail
  m_collisionManager.checkPlayerEnemyCollision(pLevel->getPlayer(), (const std::vector<GameObject*>&) m_gameObjects);
  if (!m_gameObjects.empty()) {
    for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ) {
      // if object is within screen
      if ((*it)->getPosition().getX() <= TheCamera::Instance()->getPosition().m_x + TheGame::Instance()->getGameWidth()) {
        (*it)->setUpdating(true);
        (*it)->update();
      }
      // if object offscreen
      else {
        // if that object is not Player
        if ((*it)->type() != std::string("Player"))
          (*it)->setUpdating(false);
        else
          (*it)->update(); // Player being offscreen will be updated
      }

      // if object is dead or offscreen
      if ((*it)->dead() || (*it)->getPosition().m_y > TheGame::Instance()->getGameHeight()) {
        std::cout << "deleting";
        delete *it;
        it = m_gameObjects.erase(it); // erase from vector and get a new iterator
      }
      // all ok
      else {
        ++it;
      }
    }
  }
}

void ObjectLayer::render() {
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->draw();
  }
}
// DONE