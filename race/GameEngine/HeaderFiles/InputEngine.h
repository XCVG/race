#pragma once
#include "Message.h"
#include "MessagingSystem.h"
#include "InputTypes.h"
#include <memory>

class InputEngine {
public:
    InputEngine();
    ~InputEngine();
	void buttonEventHandler(SDL_Event ev);
	void axisEventHandler(SDL_GameController*);

private:
	int16_t _moveX, _moveY;
	int16_t _lookX, _lookY;
	const int CONTROLLER_DEADZONE = 8000;
};