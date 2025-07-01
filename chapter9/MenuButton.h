#pragma once
#include "PlatformerObject.h"

class MenuButton: public PlatformerObject {
public:
  MenuButton();
  virtual ~MenuButton() {}

};

class MenuButtonCreator: public BaseCreator {
  GameObject* createGameObject() const {
    return new MenuButton();
  }
};
