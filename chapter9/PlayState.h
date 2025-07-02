#pragma once 
#include "CollisionManager.h"
#include "GameState.h"
#include "Level.h"

class PlayState: public GameState {
public:
  virtual ~PlayState() { delete pLevel; }
  virtual void update();
  virtual void render();
  virtual bool onEnter();
  virtual bool onExit();
  virtual std::string getStateID() const { return s_playID; }

private:
  static const std::string s_playID;
  CollisionManager m_collisionManager;
  std::vector<GameObject*> m_gameObjects;
  Level* pLevel;
};
// DONE