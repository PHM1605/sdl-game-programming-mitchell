#pragma once
#include "GameObjectFactory.h"
#include "PlatformerObject.h"

class ScrollingBackground: public PlatformerObject {
public:
  virtual ~ScrollingBackground() {}
  ScrollingBackground();

  virtual void load(std::unique_ptr<LoaderParams> const &pParams);
  virtual void draw();
  virtual void update();
  virtual void clean();

private:
  int m_scrollSpeed;
  int count;
  int maxcount;

  SDL_Rect m_srcRect1;
  SDL_Rect m_srcRect2;

  SDL_Rect m_destRect1;
  SDL_Rect m_destRect2;
};

class ScrollingBackgroundCreator: public BaseCreator {
public:
  virtual GameObject* createGameObject() const {
    return new ScrollingBackground();
  }
};
// DONE
