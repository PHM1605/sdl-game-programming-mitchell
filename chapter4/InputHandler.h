#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Vector2D.h"

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
	bool joysticksInitialised() {
		return m_bJoysticksInitialised;
	}

	void onJoystickAxisMove(SDL_Event& event);
private:
	InputHandler() {}
	~InputHandler() {}
	static InputHandler* s_pInstance;

	std::vector<SDL_Joystick*> m_joysticks;
	bool m_bJoysticksInitialised;
	// one 'pair' for each Joystick (=hand-held device)
	// - 1st Vector2D: left Stick (button); 2nd Vector2D: right Stick (button)
	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues; 
	int xvalue(int joy, int stick);
	int yvalue(int joy, int stick);
	const int m_joystickDeadZone = 10000; // controller sensitivity
};

typedef InputHandler TheInputHandler;

