#pragma once
#include "Turret.h"
#include "TextureManager.h"

class RoofTurret: public Turret {
public:
  virtual ~RoofTurret() {}

  virtual void draw() {
    TextureManager::Instance()->drawFrame(
      m_textureID, 
      (Uint32)m_position.getX(), (Uint32)m_position.getY(), m_width, m_height, 
      m_currentRow, m_currentFrame, 
      TheGame::Instance()->getRenderer(),
      m_angle, m_alpha, SDL_FLIP_VERTICAL
    );
  }

  virtual void update() {
    scroll(TheGame::Instance()->getScrollSpeed());
    // if RoofTurret is still alive
    if (!m_bDying) {
      if (m_bulletCounter == m_bulletFiringSpeed) {
        // Firing top down
        TheBulletHandler::Instance()->addEnemyBullet(m_position.getX(), m_position.getY()+20, 16, 16, "bullet2", 1, Vector2D(-3,3));
        TheBulletHandler::Instance()->addEnemyBullet(m_position.getX()+20, m_position.getY()+20, 16, 16, "bullet2", 1, Vector2D(0,3));
        TheBulletHandler::Instance()->addEnemyBullet(m_position.getX()+40, m_position.getY()+20, 16, 16, "bullet2", 1, Vector2D(3,3));
  
        m_bulletCounter = 0;
      }
      m_bulletCounter++;
    } 
    // or dead
    else
      doDyingAnimation();
  }
};

class RoofTurretCreator: public BaseCreator {
  GameObject* createGameObject() const {
    return new RoofTurret();
  }
};
