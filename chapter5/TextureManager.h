#pragma once
#include <iostream>
#include <map>
#include <SDL2/SDL.h>

// To make TextureManager a Singleton (i.e. class has only ONE instance)
// step 1: make its constructor/destructor private
// step 2: create a static constructing method Instance() if not exists, otherwise just use it
// step 3: create a typedef of TextureManager class at the end, calling "TheTextureManager"
// step 4: initiate it in "TextureManager.cpp"
class TextureManager {
public:
    static TextureManager* Instance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new TextureManager();
            return s_pInstance;
        }
        return s_pInstance;
    }
    bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
    void clearTextureMap();
    void clearFromTextureMap(std::string id);
    /*
    * id: id of Texture we want to draw
    * x, y: window location
    * width, height: of the image
    * currentRow, currentFrame: draw which frame on a sprite sheet
    */
    void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip=SDL_FLIP_NONE);
private:    
    TextureManager() {}
    ~TextureManager() {}
    std::map<std::string, SDL_Texture *> m_textureMap;
    static TextureManager* s_pInstance;
};

typedef TextureManager TheTextureManager;