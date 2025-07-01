#include <iostream>
#include "Game.h"

const int FPS = 60;
const int DELAY_TIME = 1000.0f/FPS; // time between 2 frames (in ms)

int main(int argc, char** argv) {
  Uint32 frameStart, frameTime;
  if (TheGame::Instance()->init("Conan the Caveman", 100, 100, 640, 480, false)) {
    std::cout << "game init success!\n";
    while (TheGame::Instance()->running()) {
      frameStart = SDL_GetTicks();
      TheGame::Instance()->handleEvents();
    }
  } else {
    std::cout << "game init failure - " << SDL_GetError() << "\n";
    return -1;
  }

  std::cout << "game closing...\n";
  TheGame::Instance()->clean();
  return 0;
}