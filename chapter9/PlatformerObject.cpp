#include "Game.h"
#include "PlatformerObject.h"
#include "TextureManager.h"
#include "TileLayer.h"

PlatformerObject::PlatformerObject():
  GameObject(),
  m_bulletFiringSpeed(0),
  m_bulletCounter(0),
  m_moveSpeed(0),
  m_dyingTime(0),
  m_dyingCounter(0),
  m_bPlayedDeathSound(false),
  m_bFlipped(false),
  m_bMoveLeft(false),
  m_bMoveRight(false),
  m_bRunning(false),
  m_bFalling(false),
  m_bJumping(false),
  m_bCanJump(false),
  m_lastSafePos(0,0)
{}

void PlatformerObject::load(std::unique_ptr<LoaderParams> const &pParams) {
  m_position = Vector2D(pParams->getX(), pParams->getY());
  m_width = pParams->getWidth();
  m_height = pParams->getHeight();
  m_textureID = pParams->getTextureID();
  m_numFrames = pParams->getNumFrames();
}

void PlatformerObject::draw() {
  TextureManager::Instance()->drawFrame(
    m_textureID, 
    (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height,
    m_currentRow, m_currentFrame,
    TheGame::Instance()->getRenderer(),
    m_angle, m_alpha
  );
}

// will be overwritten by inheriting class of PlatformerObject e.g. MenuButton::update()
// so m_numFrames is 0 in <object> in *.xml but no effect
void PlatformerObject::update() {
  m_position += m_velocity;
  m_currentFrame = int((SDL_GetTicks() / (1000/3)) % m_numFrames);
}

void PlatformerObject::doDyingAnimation() {
  m_currentFrame = int((SDL_GetTicks()/(1000/10)) % m_numFrames);
  if (m_dyingCounter == m_dyingTime) 
    m_bDead = true;
  m_dyingCounter++;
}

// we modified this function a bit, as there is nothing like a GameScreen, but we use a Camera
bool PlatformerObject::checkCollideTile(Vector2D newPos) {
  // check if Object moves outside Game screen
  if (newPos.m_y + m_height >= TheGame::Instance()->getGameHeight() - 32)
    return false;
  else {
    for (std::vector<TileLayer*>::iterator it = m_pCollisionLayers->begin(); it != m_pCollisionLayers->end(); ++it) {
      TileLayer* pTileLayer = (*it);
      std::vector<std::vector<int>> tiles = pTileLayer->getTileIDs();
      Vector2D layerPos = pTileLayer->getPosition(); // (0,0)
      
      int tileColumn, tileRow, tileid = 0;
      // startPos is a bit inside Object top-left
      // endPos is a bit inside Object bottom-right 
      Vector2D startPos = newPos;
      startPos.m_x += 15;
      startPos.m_y += 20;
      Vector2D endPos(newPos.m_x+m_width-15, newPos.m_y+m_height-4);
      // whether any point inside Object belongs to a Collidable cell
      for (int i = startPos.m_x; i < endPos.m_x; i++) {
        for (int j = startPos.m_y; j < endPos.m_y; j++) {
          tileColumn = i / pTileLayer->getTileSize();
          tileRow = j / pTileLayer->getTileSize();
          tileid = tiles[tileRow][tileColumn];
          if (tileid != 0)
            return true;
        }
      }
    }
    return false;
  }
}
// DONE