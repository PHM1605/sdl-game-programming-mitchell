#pragma once 
#include <vector>
#include "SDL2/SDL.h"
#include "Vector2D.h"

enum mouse_buttons {
  LEFT = 0,
  MIDDLE = 1,
  RIGHT = 2
};

class InputHandler {
public:
  // To make InputHandler a Singleton
  static InputHandler* Instance() {
    if (s_pInstance == nullptr)
      s_pInstance = new InputHandler();
    return s_pInstance;
  }

  void initialiseJoysticks();
  bool joysticksInitialised() const { return m_bJoysticksInitialised; }

  void reset();
  void update();
  void clean();

  // keyboard events
  bool isKeyDown(SDL_Scancode key) const;
  // mouse events
  bool getMouseButtonState(int buttonNumber) const;
  Vector2D* getMousePosition() const;
  // joysticke events
  int getAxisX(int joy, int stick) const;
  int getAxisY(int joy, int stick) const;
  bool getButtonState(int joy, int buttonNumber) const;

private:
  InputHandler();
  ~InputHandler();

  // handle keyboard events
  void onKeyDown();
  void onKeyUp();
  const Uint8* m_keystates;

  // handle mouse events
  void onMouseMove(SDL_Event& event);
  void onMouseButtonDown(SDL_Event& event);
  void onMouseButtonUp(SDL_Event& event);
  std::vector<bool> m_mouseButtonStates; // 3 bools for 3 mouse-buttons
  Vector2D* m_mousePosition;

  // handle joystick events
  void onJoystickAxisMove(SDL_Event& event);
  void onJoystickButtonDown(SDL_Event& event);
  void onJoystickButtonUp(SDL_Event& event);
  // one 'pair' for each Joystick (=hand-held device)
	// - 1st Vector2D: left Stick (button); 2nd Vector2D: right Stick (button)
  std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;
  std::vector<SDL_Joystick*> m_joysticks;
  // each Controller has a 1 bool for right Stick, 1 bool for left Stick
  std::vector<std::vector<bool>> m_buttonStates;
  bool m_bJoysticksInitialised;
  static const int m_joystickDeadZone = 10000;
  
  // Singleton
  static InputHandler* s_pInstance;
};

typedef InputHandler TheInputHandler;