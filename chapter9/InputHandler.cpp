#include <iostream>
#include "Game.h"
#include "InputHandler.h"

InputHandler* InputHandler::s_pInstance = nullptr;

InputHandler::InputHandler():
  m_keystates(nullptr),
  m_bJoysticksInitialised(false),
  m_mousePosition(new Vector2D(0, 0)) 
{
  for (int i=0; i<3; i++) 
    m_mouseButtonStates.push_back(false);
}

InputHandler::~InputHandler() {
  delete m_keystates;
  delete m_mousePosition;

  m_joystickValues.clear();
  m_joysticks.clear();
  m_buttonStates.clear();
  m_mouseButtonStates.clear();
}

void InputHandler::clean() {
  if (m_bJoysticksInitialised) {
    for (unsigned int i=0; i<SDL_NumJoysticks(); i++) {
      SDL_JoystickClose(m_joysticks[i]);
    }
  }
}

void InputHandler::initialiseJoysticks() {
  // if joystick subsystem wasn't initialised
  if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
  // if we have joystick(s) plugged in
  if (SDL_NumJoysticks() > 0) {
    for (int i=0; i<SDL_NumJoysticks(); i++) {
      SDL_Joystick* joy = SDL_JoystickOpen(i);
      // if open joystick successfully
      if (SDL_JoystickOpen(i)) {
        m_joysticks.push_back(joy);
        // 1 joystick has 1 pair, 1st Vector2D for left button, 2nd for right
        m_joystickValues.push_back(
          std::make_pair(new Vector2D(0,0), new Vector2D(0,0)));
        // for joystick buttons
        std::vector<bool> tempButtons;
        for (int j=0; j<SDL_JoystickNumButtons(joy); j++)
          tempButtons.push_back(false);
        m_buttonStates.push_back(tempButtons);
      } else {
        std::cout << SDL_GetError();
      }
    }
    // enable joystick events
    SDL_JoystickEventState(SDL_ENABLE);
    m_bJoysticksInitialised = true;
    std::cout << "Initialised " << m_joysticks.size() << " joystick(s)\n";
  } else {
    m_bJoysticksInitialised = false;
  }
}

void InputHandler::reset() {
  m_mouseButtonStates[LEFT] = false;
  m_mouseButtonStates[RIGHT] = false;
  m_mouseButtonStates[MIDDLE] = false;
}

bool InputHandler::isKeyDown(SDL_Scancode key) const {
  if (m_keystates != nullptr) {
    if (m_keystates[key] == 1) {
      return true;
    }
    else 
      return false;
  }
  return false;
}

int InputHandler::getAxisX(int joy, int stick) const {
  if (m_joystickValues.size() > 0) {
    // left stick
    if (stick == 1) 
      return m_joystickValues[joy].first->getX();
    // right stick
    else if (stick == 2)
      return m_joystickValues[joy].second->getX();
  }
  return 0;
}

int InputHandler::getAxisY(int joy, int stick) const {
  if (m_joystickValues.size() > 0) {
    // left stick
    if (stick == 1) {
      return m_joystickValues[joy].first->getY();
    }
    // right stick
    else if (stick == 2) {
      return m_joystickValues[joy].second->getY();
    }
  }
  return 0;
}

bool InputHandler::getButtonState(int joy, int buttonNumber) const {
  return m_buttonStates[joy][buttonNumber];
}

bool InputHandler::getMouseButtonState(int buttonNumber) const {
  return m_mouseButtonStates[buttonNumber];
}

Vector2D* InputHandler::getMousePosition() const {
  return m_mousePosition;
}

void InputHandler::update() {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        TheGame::Instance()->quit(); // set m_bRunning flag to false -> escape game loop
        break;
      case SDL_JOYAXISMOTION:
        onJoystickAxisMove(event);
        break;
      case SDL_JOYBUTTONDOWN:
        onJoystickButtonDown(event);
        break;
      case SDL_JOYBUTTONUP:
        onJoystickButtonUp(event);
        break;
      case SDL_MOUSEMOTION:
        onMouseMove(event);
        break;
      case SDL_MOUSEBUTTONDOWN:
        onMouseButtonDown(event);
        break;
      case SDL_MOUSEBUTTONUP:
        onMouseButtonUp(event);
        break;
      case SDL_KEYDOWN:
        onKeyDown();
        break;
      case SDL_KEYUP:
        onKeyUp();
        break;
      default:
        break;
    }
  }
}

// m_keystates: pointer to Uint, showing which key is pressed
void InputHandler::onKeyDown() {
  m_keystates = SDL_GetKeyboardState(0);
}
void InputHandler::onKeyUp() {
  m_keystates = SDL_GetKeyboardState(0);
}

void InputHandler::onMouseMove(SDL_Event& event) {
  m_mousePosition->setX(event.motion.x);
  m_mousePosition->setY(event.motion.y);
}

void InputHandler::onMouseButtonDown(SDL_Event &event) {
  if (event.button.button == SDL_BUTTON_LEFT)
    m_mouseButtonStates[LEFT] = true;
  if (event.button.button == SDL_BUTTON_MIDDLE)
    m_mouseButtonStates[MIDDLE] = true;
  if (event.button.button == SDL_BUTTON_RIGHT)
    m_mouseButtonStates[RIGHT] = true;
}

void InputHandler::onMouseButtonUp(SDL_Event &event) {
  if (event.button.button == SDL_BUTTON_LEFT)
    m_mouseButtonStates[LEFT] = false;
  if (event.button.button == SDL_BUTTON_MIDDLE)
    m_mouseButtonStates[MIDDLE] = false;
  if (event.button.button == SDL_BUTTON_RIGHT)
    m_mouseButtonStates[RIGHT] = false;
}

void InputHandler::onJoystickAxisMove(SDL_Event &event) {
  int whichOne = event.jaxis.which; // which joystick
  // axis could be 0,1,3,4
  // left stick moves left or right
  if (event.jaxis.axis == 0) {
    if (event.jaxis.value > m_joystickDeadZone) 
      m_joystickValues[whichOne].first->setX(1);
    else if (event.jaxis.value < -m_joystickDeadZone)
      m_joystickValues[whichOne].first->setX(-1);
    else 
      m_joystickValues[whichOne].first->setX(0);
  }
  // left stick moves up or down
  if (event.jaxis.axis == 1) {
    if (event.jaxis.value > m_joystickDeadZone) {
      m_joystickValues[whichOne].first->setY(1);
    } else if (event.jaxis.value < -m_joystickDeadZone)
      m_joystickValues[whichOne].first->setY(-1);
    else 
      m_joystickValues[whichOne].first->setY(0);
  }
  // right stick moves left or right
  if (event.jaxis.axis == 3) {
    if (event.jaxis.value > m_joystickDeadZone)
      m_joystickValues[whichOne].second->setX(1);
    else if (event.jaxis.value < -m_joystickDeadZone)
      m_joystickValues[whichOne].second->setX(-1);
    else 
      m_joystickValues[whichOne].second->setX(0);
  } 
  // right stick moves up or down
  if (event.jaxis.axis == 4) {
    if (event.jaxis.value > m_joystickDeadZone) 
      m_joystickValues[whichOne].second->setY(1);
    else if (event.jaxis.value < -m_joystickDeadZone)
      m_joystickValues[whichOne].second->setY(-1);
    else 
      m_joystickValues[whichOne].second->setY(0);
  }
}

void InputHandler::onJoystickButtonDown(SDL_Event& event) {
  int whichOne = event.jaxis.which;
  m_buttonStates[whichOne][event.jbutton.button] = true;
}

void InputHandler::onJoystickButtonUp(SDL_Event& event) {
  int whichOne = event.jaxis.which;
  m_buttonStates[whichOne][event.jbutton.button] = false;
}