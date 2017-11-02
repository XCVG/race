/*===================================================================================*//**
	PhysicsEngine
	
	The physics engine for the RACE game engine.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda
		
    @author Erick Fernandez de Arteaga, John Janzen
	@version 0.0.0
	@file
	
	@see PhysicsEngine
	@see PhysicsEngine.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "PhysicsEngine.h"

/*----------------------------------------------------------------------------------------
	Constructors and Destructors
----------------------------------------------------------------------------------------*/
///
///	Physics engine constructor.
///
PhysicsEngine::PhysicsEngine()
{
	subscribe(MESSAGE_TYPE::PhysicsCallMessageType);
}

///
///	Physics engine destructor.
///
PhysicsEngine::~PhysicsEngine()
{
	SDL_Log("%s", "Running Physics::Destructor");
}

/*----------------------------------------------------------------------------------------
	Instance Getter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Setter Methods
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Instance Methods
----------------------------------------------------------------------------------------*/
///
///	Starts the physics engine loop on its own thread and returns a pointer to the thread.
///
std::thread* PhysicsEngine::start()
{
	if (!_running)
	{
		_running = true;
	}

	return new std::thread(&PhysicsEngine::loop, this);
}

///
///	The physics engine loop.
///
void PhysicsEngine::loop()
{
	while (_running) {
		_urgentMessageQueueMutex_p->lock();
		_messageQueueMutex_p->lock();
		if (_messageQueue.empty() && _urgentMessageQueue.empty() && _running) 
		{
			_urgentMessageQueueMutex_p->unlock();
			_messageQueueMutex_p->unlock();
			std::this_thread::yield();
		}
		else if (!_running) 
		{
			_urgentMessageQueueMutex_p->unlock();
			_messageQueueMutex_p->unlock();
		}
		else {
			if (!_urgentMessageQueue.empty())
			{
				_messageQueueMutex_p->unlock();
				// process an urgent message


				_urgentMessageQueueMutex_p->unlock();
			}
			else
			{
				_urgentMessageQueueMutex_p->unlock();
				if (!_messageQueue.empty())
				{
					// process a normal messages
					checkMessage(_messageQueue.front());
					_messageQueue.pop();
					
				}
				_messageQueueMutex_p->unlock();
			}
		}
	}
	this->stop();
	SDL_Log("Physics::Out of loop");
}

void PhysicsEngine::checkMessage(std::shared_ptr<Message> myMessage) {
	

	switch (myMessage->getType()) {
	case MESSAGE_TYPE::PhysicsCallMessageType:
	{
		PhysicsCallMessageContent* content = static_cast<PhysicsCallMessageContent*>(myMessage->getContent());
		
		for (std::map<std::string, GameObject*>::iterator it = content->worldObjects.begin(); it != content->worldObjects.end(); ++it) {
			GameObject* go = it->second;
			generalPhysicsCall(go);
			if (it->first == "Sphere") {
				//rotate(it->second, Vector3(0, MATH_PI, 0) * content->deltaTime);
				it->second->_transform.rotate(Vector3(0, MATH_PI, 0) * content->deltaTime);
			}
		}
		_deltaTime = content->deltaTime;
		break;
	}
		break;
	break;
	default:
		break;
	}
}

void PhysicsEngine::generalPhysicsCall(GameObject* go) {
	if (go->hasComponent<AccelerationComponent*>() && go->hasComponent<VelocityComponent*>() && go->getComponent<VelocityComponent*>()->getVelocity().magnitude() > 0) 
	{
		//translate(go, go->getComponent<VelocityComponent*>()->_velocity);
		go->_transform.translate(go->getComponent<VelocityComponent*>()->_velocity);
		//if (go == _player_p && !cameraIndependant) 
		//{
		//	_camera_p->_transform._position = Vector3(*playerToCamera) + go->_transform._position;
		//}
	}
}

void PhysicsEngine::applyAcceleration(GameObject* go) {
	VelocityComponent* vc = go->getComponent<VelocityComponent*>();
	if (vc->_velocity.magnitude() < vc->_maxVelocity) {
		accelerate(go, Vector3(go->_transform._forward) * go->getComponent<AccelerationComponent*>()->_acceleration);
	}
}

/**
 *	Stops the physics engine.
 */
void PhysicsEngine::stop()
{

	//SDL_Log("Physics::Stop");
}

void PhysicsEngine::flagLoop() {
	_running = false;
}

/**
 *  <summary>
 *  Accelerate the game object. The amount should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, Vector3 amount)
{
	go->getComponent<VelocityComponent*>()->_velocity += amount;
};
/**
 * <summary>
 * Accelerate the game object. Each axis should be modified by the delta time.
 * </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<VelocityComponent*>()->_velocity += Vector3(x, y, z);
};
void PhysicsEngine::decelerate(GameObject *go, Vector3 amount)
{
	go->getComponent<AccelerationComponent*>()->_acceleration -= amount;
};
void PhysicsEngine::decelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<AccelerationComponent*>()->_acceleration -= Vector3(x, y, z);
};
