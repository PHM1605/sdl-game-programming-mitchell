#include "InputHandler.h"
#include <iostream>
#include "Game.h"

InputHandler* InputHandler::s_pInstance = nullptr;

// Joystick: handheld device
// Stick: scrolling button on the right or left of a Joystick
// Axis: left-Stick includes axis 0 (Left&Right) and 1(Up&Down); right-Stick includes axis 3 (Left&Right) and 4 (Up&Down)
void InputHandler::initialiseJoysticks() {
    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) 
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    
    if (SDL_NumJoysticks() > 0) {
        for (int i=0; i<SDL_NumJoysticks(); i++) {
            SDL_Joystick* joy = SDL_JoystickOpen(i);
            if (SDL_JoystickGetAttached(joy) == SDL_TRUE) {
                m_joysticks.push_back(joy);
                m_joystickValues.push_back(std::make_pair(new Vector2D(0,0), new Vector2D(0,0)));
            }
                
            else {
                std::cout << SDL_GetError();
            }
        }
        // Tell SDL to start listening to joystick's events
        SDL_JoystickEventState(SDL_ENABLE);
        m_bJoysticksInitialised = true;
        std::cout << "Initialised " << m_joysticks.size() << " joystick(s)\n";
    } else {
        std::cout << "No joystick detected!\n";
        m_bJoysticksInitialised = false;
    }
}

void InputHandler::update() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type){
            case SDL_QUIT:
                TheGame::Instance()->quit();
                break;
            case SDL_JOYAXISMOTION:
                onJoystickAxisMove(event);
                break;
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

void InputHandler::onJoystickAxisMove(SDL_Event& event) {
    // which controller
    int whichOne = event.jaxis.which;
    // left stick moves left or right
    if (event.jaxis.axis == 0) {
        if (event.jaxis.value > m_joystickDeadZone) {
            m_joystickValues[whichOne].first->setX(1);
        } else if (event.jaxis.value < -m_joystickDeadZone) {
            m_joystickValues[whichOne].first->setX(-1);
        } else 
            m_joystickValues[whichOne].first->setX(0);
    }
    // left stick moves up or down
    if (event.jaxis.axis == 1) {
        if (event.jaxis.value > m_joystickDeadZone) {
            m_joystickValues[whichOne].first->setY(1);
        } else if (event.jaxis.value < -m_joystickDeadZone) {
            m_joystickValues[whichOne].first->setY(-1);
        } else {
            m_joystickValues[whichOne].first->setY(0);
        }
    }
    // right stick moves left or right
    if (event.jaxis.axis == 3) {
        if (event.jaxis.value > m_joystickDeadZone) {
            m_joystickValues[whichOne].second->setX(1);
        } else if (event.jaxis.value < -m_joystickDeadZone) {
            m_joystickValues[whichOne].second->setX(-1);
        } else {
            m_joystickValues[whichOne].second->setX(0);
        }
    }
    // right stick moves up or down 
    if (event.jaxis.axis==4) {
        if (event.jaxis.value > m_joystickDeadZone) {
            m_joystickValues[whichOne].second->setY(1);
        } else if (event.jaxis.value < -m_joystickDeadZone) {
            m_joystickValues[whichOne].second->setY(-1);
        } else {
            m_joystickValues[whichOne].second->setY(0);
        }
    }
}

// get the Left/Right value; Left=1, Right=-1+
int InputHandler::xvalue(int joy, int stick) {
    if (m_joystickValues.size() > 0) {
        if (stick==1) {
            // first: Vector2D for the left stick
            return m_joystickValues[joy].first->getX();
        } else if (stick==2) {
            // second: Vector2D for the right stick
            return m_joystickValues[joy].second->getX();
        } 
    }
    return 0;
}

// get the Up/Down value; Up=1, Down=-1
int InputHandler::yvalue(int joy, int stick) {
    if (m_joystickValues.size() > 0) {
        if (stick==1) {
            // first: Vector2D for the left stick
            return m_joystickValues[joy].first->getY();
        } else if (stick==2) {
            // second: Vector2D for the right stick
            return m_joystickValues[joy].second->getY();
        }
    }
    return 0;
}