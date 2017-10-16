#pragma once
#include "SDL.h"
#include <SDL_gamecontroller.h>
#include "Message.h"
#include "MessagingSystem.h"
#include <memory>

class InputEngine {
public:
    InputEngine();
    ~InputEngine();
	void buttonEventHandler(SDL_Event ev);
	void axisEventHandler(SDL_Event ev);

private:
	SDL_GameController *gameController;
};