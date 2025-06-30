#pragma once 
#include <vector>
#include "GameObject.h"
#include "Layer.h"
#include "CollisionManager.h"

class ObjectLayer: public Layer {
public:
    virtual ~ObjectLayer();
    virtual void update(Level* pLevel);
    virtual void render();
    std::vector<GameObject*>* getGameObjects() { return &m_gameObjects; }

private:
    CollisionManager m_collisionManager;
    std::vector<GameObject*> m_gameObjects;
};