#include "ObjectLayer.h"
#include "Level.h"

ObjectLayer::~ObjectLayer() {
  for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it) {
    delete (*it);
  }
  m_gameObjects.clear();
}

void ObjectLayer::update(Level* pLevel) {
  m_collisionManager.checkPlayerEnemyCollision(pLevel->getPlayer(), (const std::vector<GameObject*>&) m_gameObjects);
  if (!m_gameObjects.empty()) {
    for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ) {
      
    }
  }
}

void ObjectLayer::render() {
  std::cout << "WERE RE";
  for (int i=0; i<m_gameObjects.size(); i++) {
    m_gameObjects[i]->draw();
  }
}