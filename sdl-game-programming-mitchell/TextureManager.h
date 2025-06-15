#pragma once
#include <iostream>
#include <string>
#include <map>
#include "SDL.h"

class TextureManager {
public:
	bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
	/*
	* id: id of Texture we want to draw
	* x, y: window location
	* width, height: of the image
	*/
	void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	std::map<std::string, SDL_Texture*> m_textureMap;

private:
	TextureManager() {}
	~TextureManager() {}
	std::map<std::string, SDL_Texture*> m_textureMap;
};
