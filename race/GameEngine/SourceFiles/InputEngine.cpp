#include "InputEngine.h"

InputEngine::InputEngine() {
	
}

InputEngine::~InputEngine() {

}

void InputEngine::buttonEventHandler(SDL_Event ev)
{
	InputMessageContent *inputContent = new InputMessageContent();
	switch (ev.cbutton.button)
	{
		case SDL_CONTROLLER_BUTTON_A:
			inputContent->buttonPressed = INPUT_TYPES::A_BUTTON;
			break;
		case SDL_CONTROLLER_BUTTON_B:
			inputContent->buttonPressed = INPUT_TYPES::B_BUTTON;
			break;
		case SDL_CONTROLLER_BUTTON_X:
			inputContent->buttonPressed = INPUT_TYPES::X_BUTTON;
			break;
		case SDL_CONTROLLER_BUTTON_Y:
			inputContent->buttonPressed = INPUT_TYPES::Y_BUTTON;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			inputContent->buttonPressed = INPUT_TYPES::DOWN_D_PAD;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			inputContent->buttonPressed = INPUT_TYPES::UP_D_PAD;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
			inputContent->buttonPressed = INPUT_TYPES::LEFT_D_PAD;
			break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
			inputContent->buttonPressed = INPUT_TYPES::RIGHT_D_PAD;
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
			inputContent->buttonPressed = INPUT_TYPES::LEFT_BUMPER;
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
			inputContent->buttonPressed = INPUT_TYPES::RIGHT_BUMBER;
			break;
		case SDL_CONTROLLER_BUTTON_LEFTSTICK:
			inputContent->buttonPressed = INPUT_TYPES::LEFT_STICK_CLICK;
			break;
		case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
			inputContent->buttonPressed = INPUT_TYPES::RIGHT_STICK_CLICK;
			
			break;
		default:
			break;
	}

	std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::InputMessageType, false));
	inputMessage->setContent(inputContent);
	MessagingSystem::instance().postMessage(inputMessage);
}

void InputEngine::axisEventHandler(SDL_Event ev)
{
	std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::InputMessageType));
	InputMessageContent *inputContent = new InputMessageContent();

	switch (ev.caxis.axis)
	{
	case SDL_CONTROLLER_AXIS_LEFTX:
		//SDL_Log("LEFT AXIS X");
		inputContent->buttonPressed = INPUT_TYPES::LEFT_ANALOG_X;
		inputContent->valueOfInput = ev.caxis.value * 0.0001;
		break;
	case SDL_CONTROLLER_AXIS_LEFTY:
		//SDL_Log("LEFT AXIS Y");
		inputContent->buttonPressed = INPUT_TYPES::LEFT_ANALOG_Y;
		inputContent->valueOfInput = ev.caxis.value * 0.0001;
		break;
	case SDL_CONTROLLER_AXIS_RIGHTX:
		//SDL_Log("RIGHT AXIS X");
		inputContent->buttonPressed = INPUT_TYPES::RIGHT_ANALOG_X;
		inputContent->valueOfInput = ev.caxis.value * 0.0001;
		break;
	case SDL_CONTROLLER_AXIS_RIGHTY:
		//SDL_Log("RIGHT AXIS Y");
		inputContent->buttonPressed = INPUT_TYPES::RIGHT_ANALOG_Y;
		inputContent->valueOfInput = ev.caxis.value * 0.0001;
		break;
	case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
		//SDL_Log("TRIGGER LEFT");
		inputContent->buttonPressed = INPUT_TYPES::LEFT_TRIGGER;
		break;
	case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
		//SDL_Log("TRIGGER RIGHT");
		inputContent->buttonPressed = INPUT_TYPES::RIGHT_TRIGGER;
		break;
	default:
		break;
	}
	
	inputMessage->setContent(inputContent);
	MessagingSystem::instance().postMessage(inputMessage);
}

