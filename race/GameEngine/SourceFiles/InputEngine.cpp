#include "InputEngine.h"

InputEngine::InputEngine() {
	SDL_Init(SDL_INIT_GAMECONTROLLER);
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
}



void InputEngine::buttonEventHandler(SDL_Event ev)
{
	/*InputMessageContent *inputContent = new InputMessageContent();
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
	MessagingSystem::instance().postMessage(inputMessage);*/
}

void InputEngine::axisEventHandler(GLfloat X, GLfloat Y, INPUT_TYPES type)
{
	InputMessageContent *inputContent = new InputMessageContent();
	inputContent->type = type;
	inputContent->lookX = X;
	inputContent->lookY = Y;

	std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::InputMessageType));
	inputMessage->setContent(inputContent);
	MessagingSystem::instance().postMessage(inputMessage);
}

void InputEngine::checkAxis(SDL_GameControllerAxis x, SDL_GameControllerAxis y, INPUT_TYPES type) {
	Sint16 degreeX = SDL_GameControllerGetAxis(gameController, x);
	Sint16 degreeY = SDL_GameControllerGetAxis(gameController, y);
	if ((degreeX < CONTROLLER_DEADZONE && degreeX > -CONTROLLER_DEADZONE) && !(degreeX > CONTROLLER_DEADZONE || degreeX < -CONTROLLER_DEADZONE))
		degreeX = 0;

	if ((degreeY < CONTROLLER_DEADZONE && degreeY > -CONTROLLER_DEADZONE) && !(degreeY > CONTROLLER_DEADZONE || degreeY < -CONTROLLER_DEADZONE))
		degreeY = 0;

	if (degreeX != 0 || degreeY != 0)
		axisEventHandler((float)degreeX / imax, (float)degreeY / imax, type);
	
}

void InputEngine::checkInput()
{
	if (gameController != nullptr) {
		checkAxis(SDL_CONTROLLER_AXIS_RIGHTX, SDL_CONTROLLER_AXIS_RIGHTY, INPUT_TYPES::LOOK_AXIS);
		checkAxis(SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY, INPUT_TYPES::MOVE_AXIS);
		checkAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, INPUT_TYPES::TRIGGERS);
	}
}

