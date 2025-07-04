#include <iostream>
#include "AnimatedGraphic.h"
#include "Game.h"
#include "GameObjectFactory.h"
#include "GameOverState.h"
#include "InputHandler.h"
#include "MainMenuState.h"
#include "MenuButton.h"
#include "Player.h"
#include "ScrollingBackground.h"
#include "Snail.h"
#include "SoundManager.h"
#include "TextureManager.h"

Game* Game::s_pInstance = 0;

Game::Game():
  m_pWindow(nullptr), 
  m_pRenderer(nullptr),
  m_bRunning(false),
  m_pGameStateMachine(nullptr),
  m_playerLives(3),
  m_bLevelComplete(false)
{
  m_levelFiles.push_back("assets/map1.tmx");
  m_currentLevel = 1;
}

Game::~Game() {
  m_pRenderer = nullptr;
  m_pWindow = nullptr;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
  m_gameWidth = width; 
  m_gameHeight = height;
  // check fullscreen or not
  int flags = 0;
  if (fullscreen)
    flags = SDL_WINDOW_FULLSCREEN;
  
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    std::cout << "SDL init success\n";
    m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (m_pWindow != 0) { // create window success
      std::cout << "window creation success\n";
      m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
      if (m_pRenderer != 0) {
        std::cout << "renderer creation success\n";
        SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
      } else {
        std::cout << "renderer init fail\n";
        return false;
      }
    } else {
      std::cout << "window init fail\n";
      return false;
    }
  } else {
    std::cout << "SDL init fail\n";
    return false;
  }

  // add some sound effects
  TheSoundManager::Instance()->load("assets/DST-Away.ogg", "music1", SOUND_MUSIC);
  TheSoundManager::Instance()->load("assets/jump.wav", "jump", SOUND_SFX);
  TheSoundManager::Instance()->playMusic("music1", -1);
  
  // add joystick
  TheInputHandler::Instance()->initialiseJoysticks();

  // register types of Objects for the Game
  TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
  TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());
  TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
  TheGameObjectFactory::Instance()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());
  TheGameObjectFactory::Instance()->registerType("Snail", new SnailCreator());

  m_pGameStateMachine = new GameStateMachine();
  m_pGameStateMachine->changeState(new MainMenuState());

  m_bRunning = true;
  return true;
}

void Game::setCurrentLevel(int currentLevel) {
  m_currentLevel = currentLevel;
  m_pGameStateMachine->changeState(new GameOverState());
  m_bLevelComplete = false;
}

void Game::render() {
  SDL_RenderClear(m_pRenderer);
  m_pGameStateMachine->render();
  SDL_RenderPresent(m_pRenderer);
}

void Game::handleEvents() {
  TheInputHandler::Instance()->update();
}

void Game::update() {
  m_pGameStateMachine->update();
}

void Game::clean() {
  std::cout << "cleaning game\n";
  // close Joysticks
  TheInputHandler::Instance()->clean(); 
  // clean StateMachine
  m_pGameStateMachine->clean();
  m_pGameStateMachine = nullptr;
  delete m_pGameStateMachine;
  // clean TextureMap
  TheTextureManager::Instance()->clearTextureMap();

  // close Window and Renderer
  SDL_DestroyWindow(m_pWindow);
  SDL_DestroyRenderer(m_pRenderer);
  SDL_Quit();
}