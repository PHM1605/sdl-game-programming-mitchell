#pragma once
#include <SDL2/SDL.h>
#include "ShooterObject.h"
#include "LoaderParams.h"
#include "GameObjectFactory.h"

class Player: public ShooterObject {
public:
	Player();
	virtual ~Player() {}
	void load(std::unique_ptr<LoaderParams> const &pParams);
	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual void collision();
	virtual std::string type() { return "Player"; }
	
private:
	// bring player back to life if there are lives left
	void ressurect();
	void handleInput();
	void handleAnimation();
	int m_invulnerable; // player invul or not (e.g. after ressurection)
	int m_invulnerableTime;
	int m_invulnerableCounter;
};

class PlayerCreator: public BaseCreator {
	GameObject* createGameObject() const {
		return new Player();
	}
};