#pragma once
#include<SDL.h>
#include <SDL_image.h>

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
	// to access private m_bRunning variable
	bool running() {
		return m_bRunning;
	}
private:
	SDL_Window* m_pWindow = 0;
	SDL_Renderer* m_pRenderer = 0;

	SDL_Texture* m_pTexture;
	SDL_Rect m_sourceRectangle;
	SDL_Rect m_destinationRectangle;

	bool m_bRunning;
};

