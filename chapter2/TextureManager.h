#pragma once
#include <iostream>
#include <map>
#include <SDL2/SDL.h>

class TextureManager {
public:
    TextureManager() {}
    ~TextureManager() {}
    bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
    /*
    * id: id of Texture we want to draw
    * x, y: window location
    * width, height: of the image
    * currentRow, currentFrame: draw which frame on a sprite sheet
    */
    void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip=SDL_FLIP_NONE);
private:    
    std::map<std::string, SDL_Texture *> m_textureMap;
};
