#include "TextureManager.h"
#include <SDL2/SDL_image.h>

TextureManager* TextureManager::s_pInstance = nullptr;

bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* pRenderer) {
    std::cout <<fileName << "AC" << std::endl;
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
    if (pTempSurface == nullptr) return false;
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
    SDL_FreeSurface(pTempSurface);
    if (pTexture != nullptr) {
        m_textureMap[id] = pTexture;
        return true;
    }
    // here means loading failed
    return false;
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;
    srcRect.x = srcRect.y = 0;
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;
    destRect.x = x;
    destRect.y = y;
    // 0,0 mean 'no rotation origin, no rotation angle'
    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
} 

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;
    srcRect.x = width * currentFrame; 
    srcRect.y = height * (currentRow-1); // row counting from 1
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;
    destRect.x = x;
    destRect.y = y;
    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

// erase the whole TextureMap
void TextureManager::clearTextureMap() {
    m_textureMap.clear();
}

// delete 1 entry
void TextureManager::clearFromTextureMap(std::string id) {
    m_textureMap.erase(id);
}

// id: Tileset name ("blocks1"), defining in map1.tmx file, being parsed in LevelParser
// x, y: destination to draw to (on the Game screen, in pixels)
// width, height: 32 pixels
void TextureManager::drawTile(
    std::string id, 
    int margin, int spacing,
    int x, int y, int width, int height,
    int currentRow, int currentFrame,
    SDL_Renderer* pRenderer
) {
    SDL_Rect srcRect;
    SDL_Rect destRect;
    srcRect.x = margin + (spacing+width)*currentFrame;
    srcRect.y = margin + (spacing+height)*currentRow;
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;
    destRect.x = x;
    destRect.y = y;
    destRect.x=0;
    destRect.y = 0;
    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, SDL_FLIP_NONE);
}