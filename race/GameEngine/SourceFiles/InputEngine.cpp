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
	InputMessageContent *inputContent = new InputMessageContent();
	switch (ev.cbutton.button)
	{
		case SDL_CONTROLLER_BUTTON_A:
			SDL_Log("A BUTTON PRESSED");
			inputContent->buttonPressed = "A Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_B:
			SDL_Log("B BUTTON PRESSED");
			inputContent->buttonPressed = "B Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_X:
			inputContent->buttonPressed = "X Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			inputContent->buttonPressed = "Y Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			inputContent->buttonPressed = "DPAD DOWN Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			inputContent->buttonPressed = "DPAD UP Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			inputContent->buttonPressed = "DPAD LEFT Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			inputContent->buttonPressed = "DPAD RIGHT Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			inputContent->buttonPressed = "LEFT SHOULDER Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			inputContent->buttonPressed = "RIGHT SHOULDER Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
			inputContent->buttonPressed = "LEFT STICK Pressed";
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
			inputContent->buttonPressed = "RIGHT STICK Pressed";
			
			break;
		default:
			break;
	}

	std::shared_ptr<Message> inputMessage = std::make_shared<Message>(MESSAGE_TYPE::InputMessageType, false);
	inputMessage->setContent(inputContent);
	MessagingSystem::instance().postMessage(inputMessage);
}

void InputEngine::axisEventHandler(SDL_Event ev)
{
	InputMessageContent *inputContent = new InputMessageContent();

	if (ev.caxis.value >= 12000 || ev.caxis.value <= -12000)
	{
		switch (ev.caxis.axis)
		{
			case SDL_CONTROLLER_AXIS_LEFTX:
				SDL_Log("LEFT AXIS X");
				inputContent->buttonPressed = "X AXIS LEFT";
				inputContent->valueOfInput = ev.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_LEFTY:
				SDL_Log("LEFT AXIS Y");
				inputContent->buttonPressed = "Y AXIS LEFT";
				inputContent->valueOfInput = ev.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_RIGHTX:
				SDL_Log("RIGHT AXIS X");
				inputContent->buttonPressed = "X AXIS RIGHT";
				inputContent->valueOfInput = ev.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_RIGHTY:
				SDL_Log("RIGHT AXIS Y");
				inputContent->buttonPressed = "Y AXIS RIGHT";
				inputContent->valueOfInput = ev.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
				SDL_Log("TRIGGER LEFT");
				inputContent->buttonPressed = "TRIGGER LEFT";
				break;
			case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
				SDL_Log("TRIGGER RIGHT");
				inputContent->buttonPressed = "TRIGGER RIGHT";
				break;
			default:
				break;
		}
	}
	std::shared_ptr<Message> inputMessage = std::make_shared<Message>(MESSAGE_TYPE::InputMessageType);
	inputMessage->setContent(inputContent);
	MessagingSystem::instance().postMessage(inputMessage);
}

