#include "Player.h"

Player::Player(const LoaderParams* pParams): 
	SDLGameObject(pParams) {}

void Player::draw() {
	SDLGameObject::draw();
}

void Player::update() {
	m_currentFrame = int((SDL_GetTicks() / 100)% 6);
	m_x -= 1; // trying to override behavior of parent i.e. moving to the right by moving to the left
}

void Player::clean() {}
