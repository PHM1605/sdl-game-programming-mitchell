#pragma once 
#include <iostream>
#include "PlatformerObject.h"

class Enemy: public PlatformerObject {
public:
  virtual std::string type() { return "Enemy"; }

protected:
  Enemy(): PlatformerObject() {}
  virtual ~Enemy() {}

  int m_health;
};
// DONE