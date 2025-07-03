#pragma once 
#include "SDL2/SDL.h"
#include "GameObject.h"

class PlatformerObject: public GameObject {
public:
  virtual ~PlatformerObject() {}

  virtual void load(std::unique_ptr<LoaderParams> const &pParams);
  virtual void update();
  virtual void draw();
  virtual void clean() {}
  virtual void collision() {} // will be overwritten in case of Player & Snail

  virtual std::string type() { return "SDLGameObject"; }

protected:
  PlatformerObject();
  // check whether 'newPos' causes a collision
  bool checkCollideTile(Vector2D newPos);
  void doDyingAnimation();

  int m_bulletFiringSpeed;
  int m_bulletCounter;
  int m_moveSpeed;

  int m_dyingTime;
  int m_dyingCounter;

  bool m_bPlayedDeathSound;
  bool m_bFlipped;
  bool m_bMoveLeft;
  bool m_bMoveRight;
  bool m_bRunning;
  bool m_bFalling;
  bool m_bJumping;
  bool m_bCanJump;
  Vector2D m_lastSafePos;
  int m_jumpHeight;
};
// DONE