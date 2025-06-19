#include "InputHandler.h"
#include <iostream>
#include "Game.h"

InputHandler* InputHandler::s_pInstance = nullptr;

void InputHandler::initialiseJoysticks() {
    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) 
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    
    if (SDL_NumJoysticks() > 0) {
        for (int i=0; i<SDL_NumJoysticks(); i++) {
            SDL_Joystick* joy = SDL_JoystickOpen(i);
            if (SDL_JoystickGetAttached(joy) == SDL_TRUE) 
                m_joysticks.push_back(joy);
            else {
                std::cout << SDL_GetError();
            }
        }
        // Tell SDL to start listening to joystick's events
        SDL_JoystickEventState(SDL_ENABLE);
        m_bJoysticksInitialised = true;
        std::cout << "Initialised " << m_joysticks.size() << " joystick(s)\n";
    } else {
        m_bJoysticksInitialised = false;
    }
}

void InputHandler::update() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            TheGame::Instance()->quit();
        }
    }
}

void InputHandler::clean() {
    if (m_bJoysticksInitialised) {
        for (unsigned int i=0; i<SDL_NumJoysticks(); i++) {
            SDL_JoystickClose(m_joysticks[i]);
        }
    }
}