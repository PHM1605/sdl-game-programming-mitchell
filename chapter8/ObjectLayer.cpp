#include "ObjectLayer.h"

void ObjectLayer::update() {
    for (int i=0; i<m_gameObjects.size(); i++) {
        m_gameObjects[i]->update();
    }
}

void ObjectLayer::render() {
    //std::cout << "OBJECT LAYER RENDERING: " << m_gameObjects[0]->getTextureID() << std::endl;
    for (int i=0; i<m_gameObjects.size(); i++) {
        m_gameObjects[i]->draw();
    }
}