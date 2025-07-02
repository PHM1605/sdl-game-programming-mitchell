#include "Player.h"

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
  // TheCamera::Instance()->setTarget(&m_position);
}

void Player::update() {

}

void Player::draw() {

}

void Player::handleAnimation() {

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
  
}

void Player::clean() {
  PlatformerObject::clean();
}

void Player::handleInput() {
  // if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT) && m_position.m_x < )
}