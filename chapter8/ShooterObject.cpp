#include "ShooterObject.h"
#include "TextureManager.h"
#include "Game.h"

ShooterObject::ShooterObject(): 
  GameObject(), 
  m_bulletFiringSpeed(0),
  m_bulletCounter(0),
  m_moveSpeed(0),
  m_dyingTime(0),
  m_dyingCounter(0),
  m_bPlayedDeathSound(false) 
  {}

void ShooterObject::load(std::unique_ptr<LoaderParams> const &pParams) {
  m_position = Vector2D(pParams->getX(), pParams->getY());
  m_width = pParams->getWidth();
  m_height = pParams->getHeight();
  m_textureID = pParams->getTextureID();
  m_numFrames = pParams->getNumFrames();
}

void ShooterObject::draw() {
  TextureManager::Instance()->drawFrame(
    m_textureID, 
    (Uint32)m_position.getX(), (Uint32)m_position.getY(), 
    m_width, m_height, 
    m_currentRow, m_currentFrame, 
    TheGame::Instance()->getRenderer(),
    m_angle, m_alpha
  );
}

void ShooterObject::update() {
  m_position += m_velocity;
  m_currentFrame = int(SDL_GetTicks() / (1000/3)) % m_numFrames;
}	

void ShooterObject::doDyingAnimation() {
  // although dying, still scroll the map
  scroll(TheGame::Instance()->getScrollSpeed());
  m_currentFrame = int(SDL_GetTicks() / (1000/3)) % m_numFrames;
  if (m_dyingCounter == m_dyingTime) 
    m_bDead = true;
  m_dyingCounter++;
}