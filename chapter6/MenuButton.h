#pragma once 
#include <SDL2/SDL.h>
#include "SDLGameObject.h"
#include "GameObjectFactory.h"

enum button_state {
  MOUSE_OUT = 0,
  MOUSE_OVER = 1,
  CLICKED = 2
};

class MenuButton: public SDLGameObject {
public:
  MenuButton();
  virtual void draw();
  virtual void update();
  virtual void clean();
  virtual void load(const LoaderParams* pParams);
  void setCallback(void (*callback)()) { m_callback = callback; }
  int getCallbackID() { return m_callbackID; }
private:
  void (*m_callback)();
  int m_callbackID; // will be set when parsing *.xml file 
  bool m_bReleased; 
};

class MenuButtonCreator: public BaseCreator {
  GameObject* createGameObject() const {
    return new MenuButton();
  }
};