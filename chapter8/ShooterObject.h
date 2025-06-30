#pragma once
#include <iostream>
#include "GameObject.h"
#include "Vector2D.h"
#include "SDL2/SDL.h"

class ShooterObject: public GameObject {
public:
  virtual ~ShooterObject() {}
  virtual void load(std::unique_ptr<LoaderParams> const &pParams);
  virtual void draw();
  virtual void update();
  virtual void clean() {}
  virtual void collision() {}
  virtual std::string type() { return "SDLGameObject"; }

protected:
  // we won't directly create ShooterObject
  ShooterObject();
  // draw the animation for the object being destroyed
  void doDyingAnimation();
  // only in "fully charged" state we can fire a Bullet i.e. m_bulletFiringSpeed == m_bulletCounter
  int m_bulletFiringSpeed;
  int m_bulletCounter;
  // how fast will this object move?
  int m_moveSpeed;
  // how long will the death animation takes? with a counter
  int m_dyingTime;
  int m_dyingCounter;
  // has the explosion sound played
  bool m_bPlayedDeathSound;
};
