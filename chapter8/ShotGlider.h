#pragma once 
#include "Glider.h"
#include "BulletHandler.h"

class ShotGlider: public Glider {
public:
  virtual ~ShotGlider() {}

  ShotGlider(): Glider() {
    m_bulletFiringSpeed = 25;
    m_moveSpeed = 3;
  }
  
  virtual void load(std::unique_ptr<LoaderParams> const &pParams) {
    ShooterObject::load(std::move(pParams));
    m_velocity.setX(-m_moveSpeed);
  }

  virtual void update() {
    if (!m_bDying) {
      if (m_bulletCounter == m_bulletFiringSpeed) {
        TheBulletHandler::Instance()->addEnemyBullet(
          m_position.getX(), m_position.getY()+15, 16, 16, "bullet2", 1, Vector2D(-10, 0)
        ); // straight back
        m_bulletCounter = 0;
      }
      m_bulletCounter++;
    } else {
        m_velocity.setX(0);
        doDyingAnimation();
    }
    ShooterObject::update();
  }
};

class ShotGliderCreator: public BaseCreator {
  GameObject* createGameObject() const {
    return new ShotGlider();
  }
};