#pragma once 
#include <iostream>
#include <vector>
#include "PlatformerObject.h"
#include "GameObjectFactory.h"

class Player: public PlatformerObject {
public:
  Player();
  virtual ~Player() {}

  virtual void load(std::unique_ptr<LoaderParams> const &pParams);
  virtual void draw();
  virtual void update();
  virtual void clean();
  
  virtual void collision();
  virtual std::string type() { return "Player"; }

private:
  void ressurect(); 
  void handleInput();
  void handleAnimation();
  void handleMovement(Vector2D velocity);

  int m_bInvulnerable;
  int m_invulnerableTime;
  int m_invulnerableCounter;

  bool m_bPressedJump;
}; 

class PlayerCreator: public BaseCreator {
  GameObject* createGameObject() const {
    return new Player();
  }
};
// DONE