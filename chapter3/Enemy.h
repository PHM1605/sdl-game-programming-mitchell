#pragma once
#include "SDLGameObject.h"
#include <SDL2/SDL.h>

class Enemy : public SDLGameObject {
public:
    Enemy(const LoaderParams* pParams);
    void draw();
    void update();
    void clean();
};