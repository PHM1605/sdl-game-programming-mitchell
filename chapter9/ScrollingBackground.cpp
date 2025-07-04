#include "Game.h"
#include "ScrollingBackground.h"
#include "TextureManager.h"

ScrollingBackground::ScrollingBackground():
  PlatformerObject()
{
  count = 0;
  maxcount = 0;
}

void ScrollingBackground::load(std::unique_ptr<LoaderParams> const &pParams) {
  PlatformerObject::load(pParams);
  m_scrollSpeed = 1;
  std::cout << m_width << std::endl;
  // source rect 1 starting location will be increased (width reduced)
  m_srcRect1.x = 0;
  m_srcRect1.y = 0;
  m_srcRect1.w = m_width; 
  m_srcRect1.h = m_height;
  // dest rect 1 will be kept at the same location (same x-y)
  m_destRect1.x = m_position.getX();
  m_destRect1.y = m_position.getY();
  m_destRect1.w = m_width;
  m_destRect1.h = m_height;
  // src rect 2 always starts at the begining of image
  m_srcRect2.x = 0;
  m_srcRect2.y = 0;
  m_srcRect2.w = 0;
  m_srcRect2.h = m_height;
  // dest rect 2 moves backward (width raises)
  m_destRect2.x = m_position.getX() + m_width;
  m_destRect2.y = m_position.getY();
  m_destRect2.w = 0;
  m_destRect2.h = m_height;
  
}

void ScrollingBackground::draw() {
  // std::cout << "DEBUG1\n";
  // std::cout << m_srcRect1.w << std::endl;
  // std::cout << m_textureID << " " << m_srcRect1.x << " " << m_srcRect1.y << " " << m_srcRect1.w << " " << m_srcRect1.h << std::endl;
  // std::cout << m_textureID << " " << m_destRect1.x << " " << m_destRect1.y << " " << m_destRect1.w << " " << m_destRect1.h << std::endl;
  // draw first rect, from start to middle (middle will move backward)
  SDL_RenderCopyEx(TheGame::Instance()->getRenderer(), TheTextureManager::Instance()->getTextureMap()[m_textureID], &m_srcRect1, &m_destRect1, 0, 0, SDL_FLIP_NONE);
  // std::cout << "DEBUG2\n";
  // std::cout << m_textureID << " " << m_srcRect2.x << " " << m_srcRect2.y << " " << m_srcRect2.w << " " << m_srcRect2.h << std::endl;
  // std::cout << m_textureID << " " << m_destRect2.x << " " << m_destRect2.y << " " << m_destRect2.w << " " << m_destRect2.h << std::endl;
  // draw second rect, from middle to end (middle will move backward)
  SDL_RenderCopyEx(TheGame::Instance()->getRenderer(), TheTextureManager::Instance()->getTextureMap()[m_textureID], &m_srcRect2, &m_destRect2, 0, 0, SDL_FLIP_NONE);
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
      m_srcRect1.x = m_srcRect1.y = 0;
      m_srcRect1.w = m_width;
      m_srcRect1.h = m_height;

      m_destRect1.x = m_position.getX();
      m_destRect1.y = m_position.getY();
      m_destRect1.w = m_width;
      m_destRect1.h = m_height;

      m_srcRect2.x = m_srcRect2.y = 0;
      m_srcRect2.w = 0;
      m_srcRect2.h = m_height;

      m_destRect2.x = m_position.getX() + m_width;
      m_destRect2.y = m_position.getY();
      m_destRect2.w = 0;
      m_destRect2.h = m_height;
    }
    count = 0;
  }
  count ++;
}

void ScrollingBackground::clean() {
  PlatformerObject::clean();
}
// DONE