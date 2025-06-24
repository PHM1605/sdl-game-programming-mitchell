#pragma once 
#include "SDLGameObject.h"
#include "LoaderParams.h"

class AnimatedGraphic: public SDLGameObject {
public:
  AnimatedGraphic();
  virtual void draw();
  virtual void update();
  virtual void clean();
  virtual void load(const LoaderParams* pParams);
private:
  int m_animSpeed;
};