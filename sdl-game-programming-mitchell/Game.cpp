#include "Game.h"
#include <iostream>

Game::Game():
	m_pWindow(0),
	m_pRenderer(0),
	m_bRunning(false)
{

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
				SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 255); // color white
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
	
	// Load image
	//SDL_Surface* pTempSurface = SDL_LoadBMP("assets/animate.bmp");
	//SDL_Surface* pTempSurface = IMG_Load("assets/animate.png");
	SDL_Surface* pTempSurface = IMG_Load("assets/animate-alpha.png"); // this image sets alpha channel to 0
	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);
	// SDL_QueryTexture(m_pTexture, NULL, NULL, &m_sourceRectangle.w, &m_sourceRectangle.h); // fill in image width/height
	// Which part of the window to draw our image on
	m_sourceRectangle.x = 0;
	m_sourceRectangle.y = 0;
	m_sourceRectangle.w = 128;
	m_sourceRectangle.h = 82;
	m_destinationRectangle.x = 0;
	m_destinationRectangle.y = 0;
	m_destinationRectangle.w = m_sourceRectangle.w;
	m_destinationRectangle.h = m_sourceRectangle.h;

	return true;
}

void Game::render() {
	// clear the window to black
	SDL_RenderClear(m_pRenderer);
	
	
	// show the image 
	SDL_RenderCopy(m_pRenderer, m_pTexture, &m_sourceRectangle, &m_destinationRectangle);
	

	/*
	//full screen
	SDL_RenderCopy(m_pRenderer, m_pTexture, 0, 0); 
	*/

	/*
	// show the image flipped
	// rotation-angle=0; center-of-rotation=0
	SDL_RenderCopyEx(m_pRenderer, m_pTexture, &m_sourceRectangle, &m_destinationRectangle, 0, 0, SDL_FLIP_HORIZONTAL);
	*/

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
	// SDL_GetTicks(): amount of ms since SDL initialized
	// 100: jump to next frame after 100ms
	m_sourceRectangle.x = 128 * int((SDL_GetTicks() / 100) % 6);
}

void Game::clean() {
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}