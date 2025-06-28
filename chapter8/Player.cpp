#include "Player.h"
#include "InputHandler.h"
#include "Game.h"

Player::Player(): ShooterObject():
	m_invulnerable(false), m_invulnerableTime(200), m_invulnerableCounter(0)
{}

void Player::load(const LoaderParams* pParams) {
	SDLGameObject::load(pParams);
}

void Player::draw() {
	SDLGameObject::draw();
}

void Player::handleAnimation() {
	// if Player is invulnerable we can flash its alpha to let people know
	if (m_invulnerable) {
		// invulnerability is finished -> set values back
		if (m_invulnerableCounter == m_invulnerableTime) {
			m_invulnerable = false;
			m_invulnerableCounter = 0;
			m_alpha = 255;
		} else { // blinking if in invulnerable state
			if (m_alpha == 255)
				m_alpha = 0;
			else 
				m_alpha = 255;
		}
		m_invulnerableCounter++;
	}	

	// if Player is not dead we change rotation according to moving direction
	if (!m_bDead) {
		if (m_velocity.getX() < 0)
			m_angle = -10.0;
		else if (m_velocity.getX() > 0)
			m_angle = 10.0;
		else 
			m_angle = 0.0;
	}

	// standard animation code - changing helicopter propellers
	m_currentFrame = int((SDL_GetTicks()/100) % m_numFrames);
}

void Player::update() {
	// if the level is complete then fly off the screen
	if (TheGame::Instance()->getLevelComplete()) {
		// Player reaches the end
		if (m_position.getX() >= TheGame::Instance()->getGameWidth())
			TheGame::Instance()->setCurrentLevel(TheGame::Instance()->getCurrentLevel()+1);
		else { // Player flies straight ahead until reaching the end
			m_velocity.setY(0);
			m_velocity.setX(3);
			ShooterObject::update(); // update position, set current frame
			handleAnimation();
		}
	} else { // level is on playing
		// if Player is not dying 
		if (!m_bDying) {
			m_velocity.setX(0);
			m_velocity.setY(0);
			handleInput();
			ShooterObject::update(); // update position based on velocity; update frame
			handleAnimation();
		} else { // Player is dying (doing dying animation)
			m_currentFrame = int((SDL_GetTicks()/100) % m_numFrames);
			// if death animation is completed
			if (m_dyingCounter == m_dyingTime)
				ressurect();
			m_dyingCounter++;
		}
	}
}

void Player::clean() {}

void Player::ressurect() {
	TheGame::Instance()->setPlayerLives(TheGame::Instance()->getPlayerLives()-1);
	m_position.setX(10);
	m_position.setY(200);
	m_bDying = false;
	m_textureID = "player";
	m_currentFrame = 0;
	m_numFrames = 5;
	m_width = 101;
	m_height = 46;
	m_dyingCounter = 0;
	m_invulnerable = true;
}

void Player::handleInput() {	
	Vector2D* target = TheInputHandler::Instance()->getMousePosition();
	m_velocity = *target - m_position;
	m_velocity /= 50;
}

