#include "Player.h"
#include "InputHandler.h"

Player::Player(const LoaderParams* pParams): 
	SDLGameObject(pParams) {}

void Player::draw() {
	SDLGameObject::draw();
}

void Player::update() {
	m_velocity.setX(0);
	m_velocity.setY(0);
	handleInput();
	m_currentFrame = int((SDL_GetTicks() / 100) % 6);
	SDLGameObject::update();
}

void Player::clean() {}

void Player::handleInput() {
	if (TheInputHandler::Instance()->joysticksInitialised()) {
		
	}
}
