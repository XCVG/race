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

///
///	Stops the physics engine.
///
void PhysicsEngine::stop()
{
    _running = false;
	//SDL_Log("Physics::Stop");
}
