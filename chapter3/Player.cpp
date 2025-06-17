#include "Player.h"

void Player::load(int x, int y, int width, int height, std::string textureID) {
	GameObject::load(x, y, width, height, textureID);
}

void Player::draw(SDL_Renderer* pRenderer) {
	GameObject::draw(pRenderer);
}

void Player::update() {
	m_x -= 1; // trying to override behavior of parent i.e. moving to the right by moving to the left
}

void Player::clean() {
	GameObject::clean();
	std::cout << "clean player";
}
