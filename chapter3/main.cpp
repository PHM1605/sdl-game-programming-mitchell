#include <SDL2/SDL.h>
#include <iostream>
#include "Game.h"

Game* g_game = nullptr;

int main(int argc, char* args[]) {
    g_game = new Game();

    if (g_game->init("Chapter 1", 100, 100, 640, 480, false)) {
        std::cout << "game init success!\n";
        // game loop
        while(g_game->running()) {
            g_game->handleEvents();
            g_game->update();
            g_game->render();
        }
    } else {
        std::cout << "game init failure - " << SDL_GetError() << "\n";
        return -1;
    }
    std::cout << "game closing...\n";
    g_game->clean();
    return 0;
}