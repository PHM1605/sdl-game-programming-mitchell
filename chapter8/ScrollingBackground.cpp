#include "ScrollingBackground.h"
#include "TextureManager.h"
#include "Game.h"

ScrollingBackground::ScrollingBackground(): ShooterObject() {
  count = 0;
  maxcount = 10;
}

void ScrollingBackground::load(std::unique_ptr<LoaderParams> const &pParams) {
  ShooterObject::load(pParams);
  // m_scrollSpeed = pParams->getAnimSpeed();
  m_scrollSpeed = 1;
  
  m_srcRect1.x = 0; 
  m_srcRect1.y = 0;
  m_srcRect1.h = m_height;
  m_srcRect1.w = m_width;

  m_destRect1.x = m_position.getX();
  m_destRect1.y = m_position.getY();
  m_srcRect1.w = m_destRect1.w = m_srcRect2Width = m_destRect2Width = m_width;
  m_srcRect1.h = m_destRect1.h = m_height;
}

void ScrollingBackground::draw() {
  // draw 1st rectangle
  SDL_RenderCopyEx(
    TheGame::Instance()->getRenderer(), 
    TheTextureManager::Instance()->getTextureMap()[m_textureID],
    &m_srcRect1, &m_destRect1,
    0, 0, SDL_FLIP_NONE
  );
  // draw 2nd rectangle
  SDL_RenderCopyEx(
    TheGame::Instance()->getRenderer(),
    TheTextureManager::Instance()->getTextureMap()[m_textureID],
    &m_srcRect2, &m_destRect2,
    0, 0, SDL_FLIP_NONE
  );
}

void ScrollingBackground::clean() {
  ShooterObject::clean();
}