#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Player.h"
#include "GameObject.h"
#include "GameStateMachine.h"

enum game_states {
    MENU = 0,
    PLAY = 1, 
    GAMEOVER = 2
};

class Game {
public:
    static Game* Instance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new Game();
            return s_pInstance;
        }
        return s_pInstance;
    }
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void render();
    void update();
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
    
    bool running () { return m_bRunning; }
    void quit() { m_bRunning = false; }
    
    int getGameWidth() const { return m_gameWidth; }
    int getGameHeight() const { return m_gameHeight; }
    float getScrollSpeed() { return m_scrollSpeed; }

    bool changingState() { return m_bChangingState; }
    void changingState(bool cs) { m_bChangingState = cs; }

    std::vector<std::string> getLevelFiles() { return m_levelFiles; }

private:
    // to make Game a Singleton
    Game();
    Game(const Game&);
    Game& operator=(const Game&);
    ~Game();

    bool m_bChangingState;
    SDL_Window* m_pWindow = 0;
    SDL_Renderer* m_pRenderer = 0;
    GameStateMachine* m_pGameStateMachine;
    bool m_bRunning;
    static Game* s_pInstance;
    int m_gameWidth;
    int m_gameHeight;
    float m_scrollSpeed;
    int m_playerLives;
    // Game levels
    int m_currentLevel;
    int m_nextLevel;
    bool m_bLevelComplete;
    std::vector<std::string> m_levelFiles;    
};

typedef Game TheGame;