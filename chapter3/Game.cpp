#include "Game.h"
#include "TextureManager.h"
#include "Enemy.h"
#include <iostream>

Game::Game():
	m_pWindow(0),
	m_pRenderer(0),
	m_bRunning(false) 
{	
	m_go = new GameObject();
	m_player = new Player();
	m_enemy = new Enemy();
	m_go->load(100, 100, 128, 82, "animate");
	m_player->load(300, 300, 128, 82, "animate");
	m_enemy->load(0, 0, 128, 82, "animate");
	m_gameObjects.push_back(m_go);
	m_gameObjects.push_back(m_player);
	m_gameObjects.push_back(m_enemy);
}

Game::~Game() {
	m_pRenderer = 0;
	m_pWindow = 0;
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;

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
				SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 255); // color white

				if (!TheTextureManager::Instance()->load("assets/animate-alpha.png", "animate", m_pRenderer)) {
					return false;
				}
			}
			else {
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

	std::cout << "init success\n";
	m_bRunning = true;

	return true;
}

void Game::render() {
	// clear the window to pre-set color
	SDL_RenderClear(m_pRenderer);
	
	for (std::vector<GameObject*>::size_type i =0; i != m_gameObjects.size(); i++) {
		m_gameObjects[i]->draw(m_pRenderer);
	}

	// show the window
	SDL_RenderPresent(m_pRenderer);
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_bRunning = false;
			break;
		default:
			break;
		}
	}
}

void Game::update() {
	for (std::vector<GameObject*>::size_type i=0; i != m_gameObjects.size(); i++) {
		m_gameObjects[i]->update();
	}
}

void Game::clean() {
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}
