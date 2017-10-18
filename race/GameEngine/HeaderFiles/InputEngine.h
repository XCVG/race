#pragma once
#include "Message.h"
#include "MessagingSystem.h"
#include "InputTypes.h"
#include <memory>

class InputEngine {
public:
    InputEngine();
    ~InputEngine();
	void buttonEventHandler(SDL_Event);
	void axisEventHandler(GLfloat, GLfloat, INPUT_TYPES);
	void checkAxis(SDL_GameControllerAxis, SDL_GameControllerAxis, INPUT_TYPES);
	void checkInput();

private:
	int16_t _moveX, _moveY;
	int16_t _lookX, _lookY;
	const int CONTROLLER_DEADZONE = 8000;
	const int imax = std::numeric_limits<__int16>::max();
	SDL_GameController *gameController;
	
};