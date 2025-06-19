#pragma once
#include <SDL2/SDL.h>
#include "SDLGameObject.h"
#include "LoaderParams.h"

class Player: public SDLGameObject {
public:
	Player(const LoaderParams* pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();
};

