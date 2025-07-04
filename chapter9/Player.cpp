#include "Camera.h"
#include "Game.h"
#include "Player.h"
#include "TextureManager.h"

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
    // if falling to the very end bottom
    if (m_position.m_y + m_height >=470)
      collision();
    
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
    if (!m_bJumping)
      m_velocity.m_y = 5;
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
  if (m_bFlipped)
    TheTextureManager::Instance()->drawFrame(
      m_textureID, 
      (Uint32)m_position.getX()-TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY()-TheCamera::Instance()->getPosition().m_y,
      m_width, m_height, m_currentRow, m_currentFrame, 
      TheGame::Instance()->getRenderer(),
      m_angle, m_alpha, SDL_FLIP_HORIZONTAL);
  else 
    TheTextureManager::Instance()->drawFrame(
      m_textureID, 
      (Uint32)m_position.getX()-TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY()-TheCamera::Instance()->getPosition().m_y,
      m_width, m_height, m_currentRow, m_currentFrame,
      TheGame::Instance()->getRenderer(),
      m_angle, m_alpha);
}

void Player::handleAnimation() {

}

void Player::handleMovement(Vector2D velocity) {

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
  // if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT) && m_position.m_x < )
}