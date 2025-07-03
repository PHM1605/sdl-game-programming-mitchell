#pragma once
#include "Camera.h"
#include "Enemy.h"
#include "Game.h"
#include "GameObjectFactory.h"
#include "TextureManager.h"

// Snail position is of whole Tilemap [pixels]
class Snail: public Enemy {
public:
  Snail(): Enemy() {
    moveLeft = true;
  }

  virtual void load(std::unique_ptr<LoaderParams> const &pParams) {
    PlatformerObject::load(std::move(pParams));
  }

  virtual void update() {
    if (moveLeft) 
      m_velocity.m_x = -2;
    else if (moveRight) 
      m_velocity.m_x = 2;
    else 
      m_velocity.m_x = 0;
    m_velocity.m_y = 5; // gravity

    handleMovement(m_velocity);
    handleAnimation();
  }

  virtual void draw() {
    if (m_flipped)
      TheTextureManager::Instance()->drawFrame(
        m_textureID, 
        (Uint32)m_position.getX()-TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY()-TheCamera::Instance()->getPosition().m_y,
        m_width, m_height, 
        m_currentRow, m_currentFrame, 
        TheGame::Instance()->getRenderer(),
        m_angle, m_alpha, SDL_FLIP_HORIZONTAL);
    else
      TheTextureManager::Instance()->drawFrame(
        m_textureID,
        (Uint32)m_position.getX()-TheCamera::Instance()->getPosition().m_x, (Uint32)m_position.getY()-TheCamera::Instance()->getPosition().m_y,
        m_width, m_height,
        m_currentRow, m_currentFrame,
        TheGame::Instance()->getRenderer(),
        m_angle, m_alpha);
  }

private:
  void handleMovement(Vector2D velocity) {
    // newPos is a 'tmp' variable to update position in x-direction; then y-direction
    Vector2D newPos = m_position;
    newPos.m_x = m_position.m_x + velocity.m_x;
    if (!checkCollideTile(newPos)) {
      m_position.m_x = newPos.m_x;
    } else {
      if (moveLeft) {
        moveLeft = false;
        moveRight = true;
      } else if (moveRight) {
        moveLeft = true;
        moveRight = false;
      }
    }

    newPos = m_position;
    newPos.m_y += velocity.m_y;
    // free falling
    if (!checkCollideTile(newPos)) {
      m_position.m_y = newPos.m_y;
    } else {
      m_velocity.m_y = 0; // standing on something
    }
  }

  void handleAnimation() {
    // moving left
    if (m_velocity.getX() < 0) {
      m_flipped = true;
    }
    // moving right
    else if (m_velocity.getX() > 0) {
      m_flipped = false;
    } 
    // standing still
    else {
      m_currentRow = 0;
      m_numFrames = 1;
    }
    m_currentFrame = int((SDL_GetTicks()/100) % m_numFrames);
  }

  bool moveLeft;
  bool moveRight;
  bool m_flipped;
};

class SnailCreator: public BaseCreator {
  GameObject* createGameObject() const {
    return new Snail();
  }
};