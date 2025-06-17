#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <TextureManager.h>

class Game {
public:
    Game();
    ~Game();

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
    
private:
    SDL_Window* m_pWindow = 0;
    SDL_Renderer* m_pRenderer = 0;
    bool m_bRunning;
    int m_currentFrame;
    TextureManager m_textureManager;
};