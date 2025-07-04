#pragma once
#include "Enemy.h"
#include "Game.h"
#include "BulletHandler.h"

class Level1Boss: public Enemy {
public:
  virtual ~Level1Boss() {}

  Level1Boss(): Enemy() {
    m_health = 100;
    m_dyingTime = 100;
    m_bulletFiringSpeed = 100;
    m_moveSpeed = 2;
    m_entered = false;
  }

  virtual void load(std::unique_ptr<LoaderParams> const &pParams) {
    ShooterObject::load(std::move(pParams));
    m_velocity.setY(-m_moveSpeed);
  }

  virtual void collision() {
    if (m_entered) {
      m_health -= 1;
      if (m_health == 0) {
        if (!m_bPlayedDeathSound) {
          m_position.setX(m_position.getX() + 30);
          m_position.setY(m_position.getY() + 70);
          TheSoundManager::Instance()->playSound("explode", 0);

          m_textureID = "bossexplosion";
          m_currentFrame = 0;
          m_numFrames = 9;
          m_width = 180;
          m_height = 180;
          m_bDying = true;
        }
      }
    }
  }

  virtual void update() {
    // have the Game screen 'met' the Boss?
    if (!m_entered) {
      // move the Object in '-' direction = move Map forward
      scroll(TheGame::Instance()->getScrollSpeed());
      // if Game-screen covers whole (Boss plus 20-pixel)
      if (m_position.getX() < (TheGame::Instance()->getGameWidth()-m_width-20)) {
        m_entered = true;
      }
    } else { // met the Boss
      // Boss alive
      if (!m_bDying) { 
        if (m_position.getY()+m_height >= TheGame::Instance()->getGameHeight())
          m_velocity.setY(-m_moveSpeed);
        else if (m_position.getY() <= 0)
          m_velocity.setY(m_moveSpeed);
        
        // shooting Bullets
        if (m_bulletCounter == m_bulletFiringSpeed) {
          TheBulletHandler::Instance()->addEnemyBullet(m_position.getX(), m_position.getY()+15, 16, 16, "bullet2", 1, Vector2D(-10,0));
          TheBulletHandler::Instance()->addEnemyBullet(m_position.getX(), m_position.getY()+25, 16, 16, "bullet2", 1, Vector2D(-10,0));
          TheBulletHandler::Instance()->addEnemyBullet(m_position.getX(), m_position.getY()+200, 16, 16, "bullet2", 1, Vector2D(-10,0));
          TheBulletHandler::Instance()->addEnemyBullet(m_position.getX(), m_position.getY()+215, 16, 16, "bullet2", 1, Vector2D(-10, 0));
          
          m_bulletCounter = 0;
        }
        m_bulletCounter++;
        ShooterObject::update();
      } 
      // dying
      else { 
        scroll(TheGame::Instance()->getScrollSpeed());
        m_currentFrame = int((SDL_GetTicks() / (1000/3)) % m_numFrames);
        if (m_dyingCounter == m_dyingTime) {
          m_bDead = true;
          TheGame::Instance()->setLevelComplete(true);
        }
        m_dyingCounter++;
      }
    }
  }

private:
  bool m_entered;
};

class Level1BossCreator: public BaseCreator {
  GameObject* createGameObject() const {
    return new Level1Boss();
  }
};