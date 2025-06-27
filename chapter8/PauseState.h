#include "MenuState.h"
#include "GameObject.h"
#include <vector>

class PauseState: public MenuState {
public:
  virtual void update();
  virtual void render();
  virtual bool onEnter();
  virtual bool onExit();
  virtual std::string getStateID() const { return s_pauseID; }

private:
  virtual void setCallbacks(const std::vector<Callback>& callbacks);
  static void s_pauseToMain(); // when pressing the "Main Menu" button
  static void s_resumePlay(); // when pressing the "Resume" button
  static const std::string s_pauseID;
  std::vector<GameObject*> m_gameObjects;
};