#pragma once
#include "SDLGameObject.h"
#include "GameObjectFactory.h"
#include <SDL2/SDL.h>

class Enemy : public SDLGameObject {
public:
    Enemy();
    void draw();
    void update();
    void clean();
    virtual void load(const LoaderParams* pParams);    
private:
    int m_numFrames;
};

class EnemyCreator: public BaseCreator {
    GameObject* createGameObject() const {
        return new Enemy();
    }
};