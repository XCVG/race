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
	void axisEventHandler(SDL_Event ev);

private:
	
};