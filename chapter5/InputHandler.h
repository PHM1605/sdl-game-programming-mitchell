#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Vector2D.h"

enum mouse_buttons {
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};

class InputHandler {
public:
	static InputHandler* Instance() {
		if (s_pInstance == 0)
			s_pInstance = new InputHandler();
		return s_pInstance;
	}

	void update();
	void clean();

	void initialiseJoysticks();
	bool joysticksInitialised() { return m_bJoysticksInitialised; }
	
	int xvalue(int joy, int stick);
	int yvalue(int joy, int stick);
	bool getButtonState(int joy, int buttonNumber) const;
	// mouse
	bool getMouseButtonState(int buttonNumber) {
		return m_mouseButtonStates[buttonNumber];
	}
	Vector2D* getMousePosition();
	// keyboard
	bool isKeyDown(SDL_Scancode key);
	void reset();
private:
	InputHandler();
	~InputHandler();
	static InputHandler* s_pInstance;

	// Joysticks axes handling
	std::vector<SDL_Joystick*> m_joysticks;
	bool m_bJoysticksInitialised;
	// one 'pair' for each Joystick (=hand-held device)
	// - 1st Vector2D: left Stick (button); 2nd Vector2D: right Stick (button)
	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues; 
	const int m_joystickDeadZone = 10000; // controller sensitivity

	// Joysticks buttons handling
	// each Controller has a 1 bool for right Stick, 1 bool for left Stick
	std::vector<std::vector<bool>> m_buttonStates;

	// Mouse buttons handling - assuming we always have only 1 mouse
	// 0: left, 1: middle, 2: right
	std::vector<bool> m_mouseButtonStates;

	// Mouse motion handling
	Vector2D* m_mousePosition;
	
	// Keyboard
	const Uint8* m_keystates;

	// handle Joystick
	void onJoystickAxisMove(SDL_Event& event);
	void onJoystickButtonDown(SDL_Event& event);
	void onJoystickButtonUp(SDL_Event& event);
	// handle keyboard
	void onKeyDown();
	void onKeyUp();
	// handle mouse
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);
};

typedef InputHandler TheInputHandler;

