#pragma once 
#include <vector>
#include <string>

class GameState {
public:
  virtual ~GameState() {} 

  virtual bool onEnter() = 0;
  virtual void update() = 0;
  virtual void render() = 0;  
  virtual bool onExit() = 0;
  virtual void resume() {}
  virtual std::string getStateID() const = 0;

protected:
  GameState(): m_loadingComplete(false), m_exiting(false) {}
  bool m_loadingComplete; // usually set to 'true' at the end of onEnter()
  bool m_exiting;
  std::vector<std::string> m_textureIDList;
};