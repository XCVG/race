#include "InputEngine.h"
#include "SDL.h"
#include <SDL_gamecontroller.h>

InputEngine::InputEngine() {
	for (int x = 0; x < SDL_NumJoysticks(); x++)
	{
		if (SDL_IsGameController(x))
		{
			gameController = SDL_GameControllerOpen(x);
			break;
		}
	}
}

InputEngine::~InputEngine() {
	if (gameController != NULL)
	{
		SDL_GameControllerClose(gameController);
	}

	gameController = nullptr;
}

void InputEngine::buttonEventHandler(SDL_Event ev)
{
	switch (ev.cbutton.button)
	{
		case SDL_CONTROLLER_BUTTON_A:
			SDL_Log("A BUTTON PRESSED");
			break;
		case SDL_CONTROLLER_BUTTON_B:
			SDL_Log("B BUTTON PRESSED");
			break;
		case SDL_CONTROLLER_BUTTON_X:
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
			break;
		default:
			break;
	}
}

void InputEngine::axisEventHandler(SDL_Event ev)
{
	switch (ev.caxis.axis)
	{
		case SDL_CONTROLLER_AXIS_LEFTX:
			SDL_Log("LEFT AXIS X");
			break;
		case SDL_CONTROLLER_AXIS_LEFTY:
			SDL_Log("LEFT AXIS Y");
			break;
		case SDL_CONTROLLER_AXIS_RIGHTX:
			SDL_Log("RIGHT AXIS X");
			break;
		case SDL_CONTROLLER_AXIS_RIGHTY:
			SDL_Log("RIGHT AXIS Y");
			break;
		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
			SDL_Log("TRIGGER LEFT");
			break;
		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			SDL_Log("TRIGGER RIGHT");
			break;
		default:
			break;
	}
}

