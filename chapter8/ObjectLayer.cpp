#include "ObjectLayer.h"
#include "Game.h"

ObjectLayer::~ObjectLayer() {
    for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it) {
        delete (*it);
    }
    m_gameObjects.clear();
}

void ObjectLayer::update(Level* pLevel) {
    m_collisionManager.checkPlayerEnemyBulletCollision(pLevel->getPlayer());
    m_collisionManager.checkEnemyPlayerBulletCollision((const std::vector<GameObject*>&) m_gameObjects);
    m_collisionManager.checkPlayerEnemyCollision(pLevel->getPlayer(), (const std::vector<GameObject*>&) m_gameObjects);
    if (pLevel->getPlayer()->getPosition().getX() + pLevel->getPlayer()->getWidth() < TheGame::Instance()->getGameWidth())
        m_collisionManager.checkPlayerTileCollision(pLevel->getPlayer(), pLevel->getCollidableLayers());

    // iterate through objects
    if (!m_gameObjects.empty()) {
        for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ) {
            // update if object is inside screen
            if ((*it)->getPosition().getX() <= TheGame::Instance()->getGameWidth()) {
                (*it)->setUpdating(true);
                (*it)->update();
            } else { // if object is outside screen
                if ((*it)->type() != std::string("Player")) {
                    (*it)->setUpdating(false);
                    (*it)->scroll(TheGame::Instance()->getScrollSpeed());
                } else {
                    (*it)->update(); 
                }
            }
            // check if dead or off screen
            if ((*it)->getPosition().getX() < -((*it)->getWidth())
                || (*it)->getPosition().getY() > TheGame::Instance()->getGameHeight()
                || (*it)->dead()) {
                delete *it;
                it = m_gameObjects.erase(it); // erase from vector and get new iterator
            } else {
                ++it; // moves to next element if all ok
            }
        }
    }
}

void ObjectLayer::render() {
    //std::cout << "OBJECT LAYER RENDERING: " << m_gameObjects[0]->getTextureID() << std::endl;
    for (int i=0; i<m_gameObjects.size(); i++) {
        if (m_gameObjects[i]->getPosition().getX() <= TheGame::Instance()->getGameWidth())
            m_gameObjects[i]->draw();
    }
}