#include "Camera.h"
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "TileLayer.h"

Player::Player():
  PlatformerObject(),
  m_bInvulnerable(false),
  m_invulnerableTime(200),
  m_invulnerableCounter(0),
  m_bPressedJump(false)
{
  m_jumpHeight = 80;
}

void Player::load(std::unique_ptr<LoaderParams> const &pParams) {
  PlatformerObject::load(std::move(pParams));
  m_bulletFiringSpeed = 13;
  m_moveSpeed = 3;
  m_bulletCounter = m_bulletFiringSpeed; // we want to fire instantly
  m_dyingTime = 100;
  TheCamera::Instance()->setTarget(&m_position);
}

void Player::update() {
  // alive
  if (!m_bDying) {
    // if falling to the very end bottom (y=470 -> within row 14)
    if (m_position.m_y + m_height >=470) {
      collision();
    }
    handleInput();

    // move left
    if (m_bMoveLeft) {
      if (m_bRunning) 
        m_velocity.m_x = -5;
      else
        m_velocity.m_x = -2;
    } 
    // move right
    else if (m_bMoveRight) {
      if (m_bRunning)
        m_velocity.m_x = 5;
      else 
        m_velocity.m_x = 2;
    } 
    // not moving
    else {
      m_velocity.m_x = 0;
    }

    // reaching max jump height -> fall down
    if (m_position.m_y < m_lastSafePos.m_y - m_jumpHeight) {
      m_bJumping = false;
    }
    // fall down if not jumping; move up otherwise
    if (!m_bJumping) {
      m_velocity.m_y = 5;
    }      
    else 
      m_velocity.m_y = -5;

    handleMovement(m_velocity);
  }
  // dying 
  else {
    m_velocity.m_x = 0;
    if (m_dyingCounter == m_dyingTime)
      ressurect();
    m_dyingCounter++;
    m_velocity.m_y = 5;
  }
  handleAnimation();
}

void Player::draw() {
  if (m_bFlipped) {
    TheTextureManager::Instance()->drawFrame(
      m_textureID, 
      (Uint32)m_position.getX()-TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY()-TheCamera::Instance()->getPosition().m_y,
      m_width, m_height, m_currentRow, m_currentFrame, 
      TheGame::Instance()->getRenderer(),
      m_angle, m_alpha, SDL_FLIP_HORIZONTAL);
  }  
  else {
    TheTextureManager::Instance()->drawFrame(
      m_textureID, 
      (Uint32)m_position.getX()-TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY()-TheCamera::Instance()->getPosition().m_y,
      m_width, m_height, m_currentRow, m_currentFrame,
      TheGame::Instance()->getRenderer(),
      m_angle, m_alpha);
  } 
   
}

void Player::handleAnimation() {
  // if Player is invulnerable we can flash its alpha to let people know
  if (m_bInvulnerable) {
    // invulnerable over -> set all values back
    if (m_invulnerableCounter == m_invulnerableTime) {
      m_bInvulnerable = false;
      m_invulnerableCounter = 0;
      m_alpha = 255;
    } else {
      if (m_alpha == 255)
        m_alpha = 0;
      else 
        m_alpha = 255;
    }
    m_invulnerableCounter++;
  }

  if (!m_bDead && !m_bDying) {
    // jumping
    if (m_velocity.m_y < 0) {
      m_currentRow = 2; 
      m_currentFrame = 2;
      m_numFrames = 2;
    } 
    // falling
    else if (m_velocity.m_y > 0) {
      m_currentRow = 3;
      m_numFrames = 1;
    }
    // on ground
    else {
      // move left
      if (m_velocity.getX() < 0) {
        m_currentRow = 1;
        m_numFrames = 4;
        m_bFlipped = true;
      }
      // move right
      else if (m_velocity.getX() > 0) {
        m_currentRow = 1;
        m_numFrames = 4;
        m_bFlipped = false;
      }
      // stand still
      else {
        m_currentRow = 0;
        m_numFrames = 1;
      }
    }

    if (m_bRunning) {
      m_currentFrame = int((SDL_GetTicks()/100) % m_numFrames); 
    } else {
      m_currentFrame = int((SDL_GetTicks()/120) % m_numFrames);
    }
  }
  // dying
  else {
    m_currentFrame = int(((SDL_GetTicks() / (200)) % m_numFrames));
  }
}

void Player::handleMovement(Vector2D velocity) {
  Vector2D newPos = m_position;
  newPos.m_x += velocity.m_x;
  if (!checkCollideTile(newPos)) {
    m_position.m_x = newPos.m_x;
  } else {
    m_velocity.m_x = 0;
  }

  newPos = m_position;
  newPos.m_y += velocity.m_y;
  if (!checkCollideTile(newPos)) {
    m_position.m_y = newPos.m_y;
  } 
  // when Player collides with something
  else {
    m_velocity.m_y = 0;
    // we collided with the map means we are safe on the ground
    m_lastSafePos = m_position;
    m_bCanJump = true;
    m_bJumping = false;
  }
}

void Player::collision() {
  if (!m_bDying && !m_bInvulnerable) {
    // converting to "Dead" sprite
    m_currentFrame = 0;
    m_currentRow = 4;
    m_numFrames = 9;
    m_width = 50; // sprite of Dead-animation is different
    m_bDying = true;
  }
}

void Player::ressurect() {
  TheGame::Instance()->setPlayerLives(TheGame::Instance()->getPlayerLives() - 1);
  m_position = m_lastSafePos;
  m_bDying = false;
  m_currentFrame = 0;
  m_currentRow = 0;
  m_width = 40;
  m_dyingCounter = 0;
  m_bInvulnerable = true;
}

void Player::clean() {
  PlatformerObject::clean();
}

void Player::handleInput() {
  // Note: -> executes first, then * (we get first vector component, Collision Tilelayer)
  // mapWidth: in [cell]
  // when we press "->"
  if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT) && m_position.m_x < ((*m_pCollisionLayers->begin())->getMapWidth()*32)) {
    // pressing "A"
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A)) 
      m_bRunning = true;
    else 
      m_bRunning = false;
    m_bMoveRight = true;
    m_bMoveLeft = false;
  } 
  // when we press "<-"
  else if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT) && m_position.m_x>32) {
    // pressing "A"
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A)) 
      m_bRunning = true;
    else 
      m_bRunning = false;
    m_bMoveRight = false;
    m_bMoveLeft = true;
  } else {
    m_bMoveRight = false;
    m_bMoveLeft = false;
  }

  // if press Spacebar & Player is on ground
  if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE) && m_bCanJump && !m_bPressedJump) {
    TheSoundManager::Instance()->playSound("jump", 0);
    m_bJumping = true;
    m_bCanJump = false;
    m_lastSafePos = m_position;
    m_bPressedJump = true; // we handle the first time pressing Jump only
  }

  // if we don't press Spacebar anymore and Player on the ground
  if (!TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_SPACE) && m_bCanJump) {
    m_bPressedJump = false;
  }
}
// DONE