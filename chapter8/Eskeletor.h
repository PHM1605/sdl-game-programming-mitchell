#pragma once
#include "BulletHandler.h"
#include "Enemy.h"
#include "Game.h"
#include "SoundManager.h"

class Eskeletor: public Enemy {
public:
  virtual ~Eskeletor() {}

  Eskeletor(): Enemy() {
    m_dyingTime = 50;
    m_health = 3;
    m_moveSpeed = 3;
    m_bulletFiringSpeed = 50;
  }

  virtual void collision() {
    m_health -= 1;
    if (m_health == 0) {
      if (!m_bPlayedDeathSound) {
        TheSoundManager::Instance()->playSound("explode", 0);
        m_textureID = "largeexplosion";
        m_currentFrame = 0;
        m_numFrames = 9;
        m_width = 60;
        m_height = 60;
        m_bDying = true;
      }
    }
  }

  virtual void update() {
    // alive
    if (!m_bDying) {
      scroll(TheGame::Instance()->getScrollSpeed());
      m_velocity.setY(m_moveSpeed);
      if (m_bulletCounter == m_bulletFiringSpeed) {
        TheBulletHandler::Instance()->addEnemyBullet(m_position.getX(), m_position.getY(), 16, 16, "bullet1", 1, Vector2D(-3,0));
        TheBulletHandler::Instance()->addEnemyBullet(m_position.getX(), m_position.getY(), 16, 16, "bullet1", 1, Vector2D(3,0));
        m_bulletCounter = 0;
      }
      m_bulletCounter++;
    } 
    // dead
    else {
      m_velocity.setY(0);
      doDyingAnimation();
    }
    ShooterObject::update();
  }
};

class EskeletorCreator: public BaseCreator {
  GameObject* createGameObject() const {
    return new Eskeletor();
  }
};