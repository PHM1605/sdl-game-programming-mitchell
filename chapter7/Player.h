#pragma once
#include <SDL2/SDL.h>
#include "SDLGameObject.h"
#include "LoaderParams.h"
#include "GameObjectFactory.h"

class Player: public SDLGameObject {
public:
	Player();
	virtual void draw();
	virtual void update();
	virtual void clean();
	void load(const LoaderParams* pParams);
private:
	void handleInput();
};

class PlayerCreator: public BaseCreator {
	GameObject* createGameObject() const {
		return new Player();
	}
};