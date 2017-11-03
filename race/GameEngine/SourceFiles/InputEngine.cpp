#include "InputEngine.h"

InputEngine::InputEngine() {
	subscribe(MESSAGE_TYPE::InputInitializeCallType);
	subscribe(MESSAGE_TYPE::InputButtonDownCallType);
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	int numJoysticks = SDL_NumJoysticks();
	if (numJoysticks != 0)
	{
		for (int x = 0; x < numJoysticks; x++)
		{
			if (SDL_IsGameController(x))
			{
				gameController = SDL_GameControllerOpen(x);
				break;
			}
		}
	}
	else
	{
		gameController = NULL;
	}
}

void InputEngine::setUpInput() 
{
	
	while (!_messageQueue.empty()) 
	{
		std::shared_ptr<Message> myMessage = _messageQueue.front();

		switch (myMessage->getType()) 
		{
			case MESSAGE_TYPE::InputInitializeCallType:
			{
				InputInitializeContent* content = static_cast<InputInitializeContent*>(myMessage->getContent());
				_camera_p = content->camera;
				_player_p = content->player;
				_playerToCamera = new Vector3(_camera_p->_transform._position.x - _player_p->_transform._position.x,
					_camera_p->_transform._position.y - _player_p->_transform._position.y,
					_camera_p->_transform._position.z - _player_p->_transform._position.z);
				GLfloat angleY = atan2(_playerToCamera->z, _playerToCamera->x);
				_camera_p->_transform._rotation.y = angleY - MATH_PI / 2;
				_messageQueue.pop();
				break;
			}
		}
	}
	
}

InputEngine::~InputEngine() 
{
	if (gameController != NULL)
	{
		SDL_GameControllerClose(gameController);
	}
}

void InputEngine::buttonEventHandler(SDL_Event ev)
{
	switch (ev.cbutton.button)
	{
		case SDL_CONTROLLER_BUTTON_Y: 
		{
			InputMessageContent *inputContent = new InputMessageContent();
			inputContent->type = INPUT_TYPES::Y_BUTTON;
			std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::InputMessageType, false));
			inputMessage->setContent(inputContent);
			MessagingSystem::instance().postMessage(inputMessage);
			break;
		}
		case SDL_CONTROLLER_BUTTON_BACK: 
		{
			cameraIndependant = !cameraIndependant;
			if (!cameraIndependant) {
				GLfloat angleY = atan2(_playerToCamera->z, _playerToCamera->x);
				_camera_p->_transform._rotation.y = angleY - MATH_PI / 2;
			}
			else {
				_camera_p->_transform._forward = Vector3(*_playerToCamera).normalize();
				_camera_p->_transform._forward.y = 0;
			}
			
			break;
		}
		default:
			break;
	}
}

void InputEngine::axisEventHandler(GLfloat X, GLfloat Y, INPUT_TYPES type)
{
	switch (type) {
	case INPUT_TYPES::LOOK_AXIS:
	{
		//SDL_Log("A Button Pressed");
		if (this->cameraIndependant)
		{
			//rotate(_camera_p, Vector3(content->lookY, content->lookX, 0) * 2 * _deltaTime);
			_camera_p->_transform.rotate(Vector3(Y, X, 0) * 2 * _deltaTime);
		}
		else
		{
			glm::mat4x4 matrix = glm::eulerAngleXYZ(0.0f, -X * _deltaTime, 0.0f);
			Vector3 tempVect = Vector3(*_playerToCamera).matrixMulti(matrix);

			tempVect += _player_p->_transform._position;
			_camera_p->_transform._position = tempVect;
			_playerToCamera = new Vector3(_camera_p->_transform._position.x - _player_p->_transform._position.x,
				_camera_p->_transform._position.y - _player_p->_transform._position.y,
				_camera_p->_transform._position.z - _player_p->_transform._position.z);
			GLfloat angleY = atan2(_playerToCamera->z, _playerToCamera->x);
			//GLfloat angleX = acosf(Vector3(*_playerToCamera).dotProduct(_player_p->_transform._position) /
				//_camera_p->_transform._position.magnitude() * _player_p->_transform._position.magnitude());
			//GLfloat angleX = atan2(_playerToCamera->z, _playerToCamera->x) - atan2(_player_p->_transform._position.z, _player_p->_transform._position.x);
			//rotate(_camera_p, Vector3(0, -content->lookX, 0) * _deltaTime);

			_camera_p->_transform._rotation.y = angleY - MATH_PI / 2;
			//_camera_p->_transform._rotation.x = angleX - MATH_PI / 2;
		}

		break;
	}
	case INPUT_TYPES::MOVE_AXIS:
	{
		if (this->cameraIndependant)
		{
			glm::mat4x4 matrix = glm::eulerAngleXYZ(_camera_p->_transform.getRotation().x, _camera_p->_transform.getRotation().y, _camera_p->_transform.getRotation().z);
			//translate(_camera_p, Vector3(content->lookX, 0, content->lookY).matrixMulti(matrix) * 2 * _deltaTime);
			_camera_p->_transform.translate(Vector3(X, 0, Y).matrixMulti(matrix) * 2 * _deltaTime);
			//_camera_p->_transform.translateRight(X * 15 * _deltaTime);
		}
		else
		{

		}

	}
	break;
	case INPUT_TYPES::TRIGGERS:
	{
		
		PhysicsAccelerateContent *content = new PhysicsAccelerateContent();
		content->amount = Y;
		content->object = _player_p;
		std::shared_ptr<Message> inputMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::PhysicsAccelerateCallType, false));
		inputMessage->setContent(content);
		MessagingSystem::instance().postMessage(inputMessage);
	}
	break;
	default:
		break;
	}
}

void InputEngine::checkAxis(SDL_GameControllerAxis x, SDL_GameControllerAxis y, INPUT_TYPES type) 
{
	int16_t degreeX = SDL_GameControllerGetAxis(gameController, x);
	int16_t degreeY = SDL_GameControllerGetAxis(gameController, y);
	if ((degreeX < CONTROLLER_DEADZONE && degreeX > -CONTROLLER_DEADZONE) && !(degreeX > CONTROLLER_DEADZONE || degreeX < -CONTROLLER_DEADZONE))
		degreeX = 0;

	if ((degreeY < CONTROLLER_DEADZONE && degreeY > -CONTROLLER_DEADZONE) && !(degreeY > CONTROLLER_DEADZONE || degreeY < -CONTROLLER_DEADZONE))
		degreeY = 0;

	if (degreeX != 0 || degreeY != 0)
		axisEventHandler((float)degreeX / imax, (float)degreeY / imax, type);
	
}

void InputEngine::checkInput(GLfloat deltaTime)
{
	_deltaTime = deltaTime; 
	while (!_messageQueue.empty())
	{
		std::shared_ptr<Message> myMessage = _messageQueue.front();

		switch (myMessage->getType())
		{
			case MESSAGE_TYPE::InputButtonDownCallType:
			{
				InputButtonDownContent* content = static_cast<InputButtonDownContent*>(myMessage->getContent());
				buttonEventHandler(content->ev);
				_messageQueue.pop();
			}
		}
	}
	if (gameController != NULL) {
        checkAxis(SDL_CONTROLLER_AXIS_RIGHTX, SDL_CONTROLLER_AXIS_RIGHTY, INPUT_TYPES::LOOK_AXIS);
        checkAxis(SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY, INPUT_TYPES::MOVE_AXIS);
        checkAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, INPUT_TYPES::TRIGGERS);
    }
	if (!cameraIndependant) {
		_camera_p->_transform._position = Vector3(*_playerToCamera) + _player_p->_transform._position;
	}
}