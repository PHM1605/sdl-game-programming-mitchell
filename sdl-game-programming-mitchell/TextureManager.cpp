#include "TextureManager.h"
#include "SDL_image.h"

bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* pRenderer) {
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
	if (pTempSurface == 0) {
		return false;
	}
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);
	// if all OK
	if (pTexture != 0) {
		m_textureMap[id] = pTexture;
		return true;
	}
	return false;
}
