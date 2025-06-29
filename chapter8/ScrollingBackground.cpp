#include "ScrollingBackground.h"
#include "TextureManager.h"
#include "Game.h"

// ScrollingBackground principle: split background to 2 parts
// 1st rectangle part will decrease over time
// 2nd rectangle part will increase over time
ScrollingBackground::ScrollingBackground(): ShooterObject() {
  count = 0;
  maxcount = 10;
}

void ScrollingBackground::load(std::unique_ptr<LoaderParams> const &pParams) {
  ShooterObject::load(pParams);
  // m_scrollSpeed = pParams->getAnimSpeed();
  m_scrollSpeed = 1;
  // source 1: whole texture map
  m_srcRect1.x = 0; 
  m_srcRect1.y = 0;
  m_srcRect1.w = m_width;
  m_srcRect1.h = m_height;
  // destination 1 (on screen)
  m_destRect1.x = m_position.getX();
  m_destRect1.y = m_position.getY();
  m_destRect1.w = m_width;
  m_destRect1.h = m_height;
  // source 2
  m_srcRect2.x = 0;
  m_srcRect2.y = 0;
  m_srcRect2.w = 0;
  m_srcRect2.h = m_height;
  // destination 2 (on screen)
  m_destRect2.x = m_position.getX() + m_width;
  m_destRect2.y = m_position.getY();
  m_destRect2.w = 0;
  m_destRect2.h = m_height;
}

void ScrollingBackground::draw() {
  // draw a rectangle at an offset (x,y)
  SDL_RenderCopyEx(
    TheGame::Instance()->getRenderer(), 
    TheTextureManager::Instance()->getTextureMap()[m_textureID],
    &m_srcRect1, &m_destRect1,
    0, 0, SDL_FLIP_NONE
  );
  // draw a slice of rectangle at an offset (x+w,y)
  SDL_RenderCopyEx(
    TheGame::Instance()->getRenderer(),
    TheTextureManager::Instance()->getTextureMap()[m_textureID],
    &m_srcRect2, &m_destRect2,
    0, 0, SDL_FLIP_NONE
  );
}

void ScrollingBackground::update() {
  if (count == maxcount) {
    // make 1st rectangle smaller
    m_srcRect1.x += m_scrollSpeed;
    m_srcRect1.w -= m_scrollSpeed;
    m_destRect1.w -= m_scrollSpeed;
    // make 2nd rectangle bigger
    m_srcRect2.w += m_scrollSpeed;
    m_destRect2.w += m_scrollSpeed;
    m_destRect2.x -= m_scrollSpeed;
    // reset and start again
    if (m_destRect2.w >= m_width) {
      m_srcRect1.x = 0;
      m_srcRect1.y = 0;
      m_srcRect1.w = m_width;
      m_srcRect1.h = m_height;

      m_destRect1.x = m_position.getX();
      m_destRect1.y = m_position.getY();
      m_destRect1.w = m_width;
      m_destRect1.h = m_height;

      m_srcRect2.x = 0;
      m_srcRect2.y = 0;
      m_srcRect2.w = 0;
      m_srcRect2.h = m_height;

      m_destRect2.x = m_position.getX() + m_width;
      m_destRect2.y = m_position.getY();
      m_destRect2.w = 0;
      m_destRect2.h = m_height;
    }
    count = 0;
  }
  count++;
}

void ScrollingBackground::clean() {
  ShooterObject::clean();
}