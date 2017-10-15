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
	this->stop();
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
		if (_messageQueue.empty() && _urgentMessageQueue.empty() && _running) {
			_urgentMessageQueueMutex_p->unlock();
			_messageQueueMutex_p->unlock();
			std::this_thread::yield();
		}
		else {
			if (!_urgentMessageQueue.empty()) {
				_messageQueueMutex_p->unlock();
				// process an urgent message


				_urgentMessageQueueMutex_p->unlock();
			}
			else
			{
				_urgentMessageQueueMutex_p->unlock();
				if (!_messageQueue.empty())
				{
					std::shared_ptr<Message> myMessage = _messageQueue.front();
					PhysicsCallMessageContent* content = static_cast<PhysicsCallMessageContent*>(myMessage->getContent());
					GameObject* go = content->go;
					rotate(go, Vector3(0.2, 0.3, 0.5));
					//SDL_Log(content->contentVar.c_str());
					// process a normal message
					_messageQueue.pop();
					//SDL_Log("After pop()");

					//SDL_Log("Message Processed");
					
				}
				_messageQueueMutex_p->unlock();
			}
		}
	}
	//SDL_Log("Physics::Out of loop");
}

/**
 *	Stops the physics engine.
 */
void PhysicsEngine::stop()
{
    _running = false;
	//SDL_Log("Physics::Stop");
}
/**
 *  <summary>
 *  Move the game object in a direciton. The translation should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::translate(GameObject *go, Vector3 translation)
{
	go->_transform._position = translation;
};
/**
 *  <summary>
 *  Move the game object in a direciton. Each axis should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::translate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->_transform._position = Vector3(x, y, z);
};
/**
 *  <summary>
 *  Accelerate the game object. The amount should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, Vector3 amount)
{
	go->getComponent<AccelerationComponent*>()->_acceleration += amount;
};
/**
 * <summary>
 * Accelerate the game object. Each axis should be modified by the delta time.
 * </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<AccelerationComponent*>()->_acceleration += Vector3(x, y, z);
};
void PhysicsEngine::decelerate(GameObject *go, Vector3 amount)
{
	go->getComponent<AccelerationComponent*>()->_acceleration -= amount;
};
void PhysicsEngine::decelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<AccelerationComponent*>()->_acceleration -= Vector3(x, y, z);
};
/**
 * <summary>
 * Rotate the object by a set amount. This rotation is in radians only, 
 * and is only ever increasing the rotation.
 * Please specify positive/negative when calling. e.g., rotate(&go, -1.2);
 * </summary>
 */
void PhysicsEngine::rotate(GameObject *go, Vector3 amount)
{
	go->_transform._rotation += amount;
};
void PhysicsEngine::rotateX(GameObject *go, GLfloat angle)
{
	go->_transform._rotation.x += angle;
};
void PhysicsEngine::rotateY(GameObject *go, GLfloat angle)
{
	go->_transform._rotation.y += angle;
};
void PhysicsEngine::rotateZ(GameObject *go, GLfloat angle)
{
	go->_transform._rotation.z += angle;
};