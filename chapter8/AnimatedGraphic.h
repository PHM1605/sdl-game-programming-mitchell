#pragma once 
#include "ShooterObject.h"
#include "LoaderParams.h"
#include "GameObjectFactory.h"

class AnimatedGraphic: public ShooterObject {
public:
  AnimatedGraphic();
  virtual void draw();
  virtual void update();
  virtual void clean();
  virtual void load(std::unique_ptr<LoaderParams> const &pParams);
private:
  int m_animSpeed;
};

class AnimatedGraphicCreator: public BaseCreator {
public:
  GameObject* createGameObject() const {
    return new AnimatedGraphic();
  }
};