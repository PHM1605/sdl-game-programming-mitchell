#pragma once 
#include "SDL2/SDL.h"
#include "GameStateMachine.h"

class Game {
public:
  // to make Game a Singleton
  static Game* Instance() {
    if (s_pInstance == nullptr) {
      s_pInstance = new Game();
      return s_pInstance;
    }
    return s_pInstance;
  }

  bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
  void update();
  void render();
  void handleEvents();
  void clean();

  SDL_Renderer* getRenderer() const { return m_pRenderer; }
  SDL_Window* getWindow() const { return m_pWindow; }
  GameStateMachine* getStateMachine() { return m_pGameStateMachine; }

  void setPlayerLives(int lives) { m_playerLives = lives; }
  int getPlayerLives() { return m_playerLives; }

  void setCurrentLevel(int currentLevel);
  const int getCurrentLevel() { return m_currentLevel; }
  void setNextLevel(int nextLevel) { m_nextLevel = nextLevel; }
  const int getNextLevel() { return m_nextLevel; }
  void setLevelComplete(bool levelComplete) { m_bLevelComplete = levelComplete; }
  const bool getLevelComplete() { return m_bLevelComplete; }

  bool running() { return m_bRunning; }
  void quit() { m_bRunning = false; }

  int getGameWidth() const { return m_gameWidth; }
  int getGameHeight() const { return m_gameHeight; }

  std::vector<std::string> getLevelFiles() { return m_levelFiles; }

private:
  SDL_Window* m_pWindow;
  SDL_Renderer* m_pRenderer;
  GameStateMachine* m_pGameStateMachine;
  bool m_bRunning;
  static Game* s_pInstance;
  // size of Game screen
  int m_gameWidth;
  int m_gameHeight;
  
  int m_playerLives;
  int m_currentLevel;
  int m_nextLevel;
  bool m_bLevelComplete;
  std::vector<std::string> m_levelFiles;
  Game();
  ~Game();
};

typedef Game TheGame;