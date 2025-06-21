#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <TextureManager.h>
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

    SDL_Renderer* getRenderer() const { return m_pRenderer; }

    int m_gameWidth;
    int m_gameHeight;
    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void render();
    void update();
    void handleEvents();
    void clean();
    bool running () {
        return m_bRunning;
    }
    void quit() { m_bRunning = false; }
    
private:
    // to make Game a Singleton
    Game() {}
    ~Game() {}
    static Game* s_pInstance;

    SDL_Window* m_pWindow = 0;
    SDL_Renderer* m_pRenderer = 0;
    bool m_bRunning;
    int m_currentFrame;
    std::vector<GameObject*> m_gameObjects;
    GameStateMachine* m_pGameStateMachine;
};

typedef Game TheGame;