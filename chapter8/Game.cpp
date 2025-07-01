#include "AnimatedGraphic.h"
#include "Eskeletor.h"
#include "Game.h"
#include "GameObjectFactory.h"
#include "GameOverState.h"
#include "Glider.h" 
#include "InputHandler.h"
#include "Level1Boss.h"
#include "MainMenuState.h"
#include "MenuButton.h"
#include "Player.h"
#include "RoofTurret.h"
#include "ScrollingBackground.h"
#include "ShotGlider.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "Turret.h"

#include <iostream>

// as Game is Singleton
Game* Game::s_pInstance = 0;

Game::Game():
	m_pWindow(0),
	m_pRenderer(0),
	m_bRunning(false),
	m_pGameStateMachine(0),
	m_playerLives(3),
	m_scrollSpeed(0.8),
	m_bLevelComplete(false),
	m_bChangingState(false) {
		m_levelFiles.push_back("assets/map1.tmx");
		m_levelFiles.push_back("assets/map2.tmx");
		m_currentLevel = 1;
	}

Game::~Game() {
	m_pRenderer = 0;
	m_pWindow = 0;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	m_gameWidth = width;
	m_gameHeight = height;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL init success\n";
		// if successfully create our window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != 0) {
			std::cout << "window creation success\n";
			// -1: index of rendering driver
			// 0: SDL_RendererFlag
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			// renderer init success
			if (m_pRenderer != 0) {
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer, 41, 118, 187, 255); // color black
			} else {
				std::cout << "renderer init fail\n";
				return false;
			}
		}
		else {
			std::cout << "window init fail\n";
			return false;
		}
	}
	else {
		std::cout << "SDL init fail\n";
		return false;
	}

	// // Init Input (Joystick, ...)
	// TheInputHandler::Instance()->initialiseJoysticks();
	
	// add sound effects
	TheSoundManager::Instance()->load("assets/DST_ElectroRock.ogg", "music1", SOUND_MUSIC);
	TheSoundManager::Instance()->load("assets/boom.wav", "explode", SOUND_SFX);
	TheSoundManager::Instance()->load("assets/phaser.wav", "shoot", SOUND_SFX);
	TheSoundManager::Instance()->playMusic("music1", -1);

	// Register Objects for the Game
	TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
	TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());
	TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());
	TheGameObjectFactory::Instance()->registerType("ScrollingBackground", new ScrollingBackgroundCreator());
	TheGameObjectFactory::Instance()->registerType("Turret", new TurretCreator());
	TheGameObjectFactory::Instance()->registerType("Glider", new GliderCreator());
	TheGameObjectFactory::Instance()->registerType("ShotGlider", new ShotGliderCreator());
	TheGameObjectFactory::Instance()->registerType("RoofTurret", new RoofTurretCreator());
	TheGameObjectFactory::Instance()->registerType("Eskeletor", new EskeletorCreator());
	TheGameObjectFactory::Instance()->registerType("Level1Boss", new Level1BossCreator());
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
	// clear the window to pre-set color
	SDL_RenderClear(m_pRenderer);	
	// render GameStateS
	m_pGameStateMachine->render();
	// show the window
	SDL_RenderPresent(m_pRenderer);
}


void Game::update() {
	m_pGameStateMachine->update();
}

void Game::handleEvents() {
	TheInputHandler::Instance()->update();
}

void Game::clean() {
	std::cout << "cleaning game\n";
	TheInputHandler::Instance()->clean();
	m_pGameStateMachine = nullptr;
	delete m_pGameStateMachine;
	TheTextureManager::Instance()->clearTextureMap();

	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}
