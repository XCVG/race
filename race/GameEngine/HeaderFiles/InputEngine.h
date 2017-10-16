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
	void lookEventHandler(Sint16 X, Sint16 Y);

private:
	int16_t _moveX, _moveY;
	int16_t _lookX, _lookY;
	
};