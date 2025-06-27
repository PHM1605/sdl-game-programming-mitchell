#include "GameState.h"
#include "GameObject.h"
#include "SDLGameObject.h"
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
  bool checkCollision(SDLGameObject* p1, SDLGameObject* p2);
  Level* pLevel;
};