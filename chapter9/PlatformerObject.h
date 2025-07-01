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
  virtual void collision() {}
  virtual std::string type() { return "SDLGameObject"; }
};