#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include "SoundManager.h"
#include "BulletHandler.h"

Player::Player(): 
	ShooterObject(), m_invulnerable(false), m_invulnerableTime(200), m_invulnerableCounter(0)
{}

void Player::collision() {
	// if Player is not invulnerable then set to dying and change values for death animation tile sheet
	if (!m_invulnerable && !TheGame::Instance()->getLevelComplete()) {
		m_textureID = "largeexplosion";
		m_currentFrame = 0;
		m_numFrames = 9;
		m_width = 60;
		m_height = 60;
		m_bDying = true;
	}
}

void Player::load(std::unique_ptr<LoaderParams> const &pParams) {
	ShooterObject::load(std::move(pParams)); // pParams now belongs to ShooterObject (we won't use pParams later)
	// Setup Player's inherited values (from ShooterObject) here
	m_bulletFiringSpeed = 13;
	m_moveSpeed = 3;
	m_bulletCounter = m_bulletFiringSpeed; 
	m_dyingTime = 100; // time it takes for death explosion
}

void Player::draw() {
	ShooterObject::draw();
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

void Player::clean() {
	ShooterObject::clean();
}

void Player::handleInput() {	
	if (!m_bDead) {
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP) && m_position.getY() > 0)
			m_velocity.setY(-m_moveSpeed);
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN) && (m_position.getY()+m_height) < TheGame::Instance()->getGameHeight()-10)
			m_velocity.setY(m_moveSpeed);
		
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT) && m_position.getX() > 0)
			m_velocity.setX(-m_moveSpeed);
		else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT) && (m_position.getX()+m_width) < TheGame::Instance()->getGameWidth())
			m_velocity.setX(m_moveSpeed);
		
		if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE)) {
			if (m_bulletCounter = m_bulletFiringSpeed) {
				TheSoundManager::Instance()->playSound("shoot", 0);
				TheBulletHandler::Instance()->addPlayerBullet(
					m_position.getX()+90, m_position.getY()+12, 11, 11, "bullet1", 1, Vector2D(10,0)
				);
				m_bulletCounter = 0;
			}
			m_bulletCounter++;
		} else {
			m_bulletCounter = m_bulletFiringSpeed;
		}
	}
}

