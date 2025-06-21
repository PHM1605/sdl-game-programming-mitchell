#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int DIALOG_BOX_HEIGHT = 100;
const Uint32 CHAR_DELAY_MS = 50; // Delay between each character

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() == -1) return false;
    window = SDL_CreateWindow("Dialog Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;
    font = TTF_OpenFont("assets/fonts/monogram.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
        return false;
    }
    return true;
}

void close() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

std::string wrapTextToFit(const std::string& text, int maxWidth) {
    std::istringstream iss(text);
    std::string word;
    std::string line, wrapped;

    while(iss >> word) {
        std::string candidate = line.empty() ? word : line+" "+word;
        // measure the size of text in pixels (w=#pixels)
        int w = 0; 
        TTF_SizeText(font, candidate.c_str(), &w, nullptr);
        // if pixel-width is too big
        if (w > maxWidth) {
            if (!wrapped.empty()) wrapped += "\n";
            wrapped += line; // 'wrapped' is the candidate minus the exceeding word
            line = word; // 'line' is the exceeding word should be new-lined
        } else 
            line = candidate;
    }

    if (!line.empty()) {
        if (!wrapped.empty()) wrapped += '\n';
        wrapped += line;
    }
    return wrapped;
}

void renderDialogBox(const std::string& text, int charsToShow) {
    // 20 is the border (in pixels)
    SDL_Rect dialogRect = {20, SCREEN_HEIGHT-DIALOG_BOX_HEIGHT-20, SCREEN_WIDTH-40, DIALOG_BOX_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // black box
    SDL_RenderFillRect(renderer, &dialogRect);

    SDL_Color textColor = {255, 255, 255}; // white chars
    std::string visibleText = text.substr(0, charsToShow);
    // split the wrapped long text to individual sentences
    std::istringstream lineStream(visibleText);
    std::string line;
    int lineHeight = TTF_FontHeight(font);
    int y = dialogRect.y + 10; // y of the next line
    while (std::getline(lineStream, line)) {
        SDL_Surface* surface = TTF_RenderText_Blended(font, line.c_str(), textColor);
        if (!surface) continue;
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            continue;
        }
        
        SDL_Rect textRect = {dialogRect.x + 10, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        y += lineHeight;
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

int main(int argc, char* argv[]) {
    if (!init()) {
        std::cerr << "Init failed!\n";
        return 1;
    }
    std::string fullText = "Hey there! Welcome to our world. It's full of wonders and adventures!";
    std::string wrappedText = wrapTextToFit(fullText, SCREEN_WIDTH - 40 - 20);
    bool quit = false;
    SDL_Event e;
    Uint32 lastCharTime = SDL_GetTicks();
    int charsShown = 1;

    while(!quit) {
        // if pressing 'X' (quit button)
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }

        Uint32 currentTime = SDL_GetTicks();
        if (charsShown < fullText.length() && currentTime - lastCharTime > CHAR_DELAY_MS) {
            charsShown++;
            lastCharTime = currentTime;
        }

        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255); // Clear screen (blueish)
        SDL_RenderClear(renderer);
        renderDialogBox(wrappedText, charsShown);
        SDL_RenderPresent(renderer);
    }
    close();

    return 0;
}