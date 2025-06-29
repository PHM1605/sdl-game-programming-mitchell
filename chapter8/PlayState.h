#include "GameState.h"
#include "CollisionManager.h"
#include "Level.h"
#include <vector>

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
  std::vector<GameObject*> m_gameObjects;
  Level* pLevel;
};