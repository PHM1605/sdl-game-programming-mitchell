#include <iostream>
#include <string>
#include "TextureManager.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"

TextureManager* TextureManager::s_pInstance = 0;

TextureManager::~TextureManager() {
  clearTextureMap();
}

bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* pRenderer) {
  SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
  if (pTempSurface == 0) {
    std::cout << IMG_GetError();
    return false; 
  }
  SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
  SDL_FreeSurface(pTempSurface);
  if (pTexture != 0) {
    m_textureMap[id] = pTexture;
    return true;
  }
  return false;
}

// x, y: destination to draw to (on the Game screen, in pixels)
// width, height: in pixels
void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip){
  SDL_Rect srcRect;
  SDL_Rect destRect;
  
  srcRect.x = 0;
  srcRect.y = 0;
  srcRect.w = destRect.w = width;
  srcRect.h = destRect.h = height;
  destRect.x = x;
  destRect.y = y;
  // 0,0 mean 'no rotation origin, no rotation angle'
  SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}
// x, y: destination to draw to (on the Game screen, in pixels)
// width, height: 1 Tile shape, small, in [pixels]
void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, double angle, int alpha, SDL_RendererFlip flip) {
  SDL_Rect srcRect;
  SDL_Rect destRect; 
  srcRect.x = width * currentFrame;
  srcRect.y = height * currentRow;
  srcRect.w = destRect.w = width;
  srcRect.h = destRect.h = height;
  destRect.x = x;
  destRect.y = y;

  SDL_SetTextureAlphaMod(m_textureMap[id], alpha);
  SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, angle, 0, flip);
}

// id: Tileset name ("prehistoric"), defining in map1.tmx file, being parsed in LevelParser
// x, y: destination to draw to (on the Game screen, in pixels)
// width, height: 32 pixels
void TextureManager::drawTile(std::string id, int margin, int spacing, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer) {
  SDL_Rect srcRect;
  SDL_Rect destRect;
  srcRect.x = margin + (spacing+width)*currentFrame;
  srcRect.y = margin + (spacing+height)*currentRow;
  srcRect.w = destRect.w = width;
  srcRect.h = destRect.h = height;
  destRect.x = x;
  destRect.y = y;

  SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}

void TextureManager::clearTextureMap() {
  for (auto& pair: m_textureMap) {
    SDL_DestroyTexture(pair.second);
  }
  m_textureMap.clear();
}

void TextureManager::clearFromTextureMap(std::string id) {
  auto it = m_textureMap.find(id);
  if (it != m_textureMap.end()) {
    SDL_DestroyTexture(it->second);
    m_textureMap.erase(id);
  }  
}

void TextureManager::clean() {
  if (s_pInstance) {
    delete s_pInstance;
    s_pInstance = nullptr;
  }
}

// DONE