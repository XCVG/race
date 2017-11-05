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
	subscribe(MESSAGE_TYPE::PhysicsAccelerateCallType);
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
			go->_lockMutex.lock();
			generalPhysicsCall(go);
			
			if (it->first == "Sphere") {
				it->second->_transform.rotate(Vector3(0, MATH_PI / 2, 0) * content->deltaTime);
			}
			go->_lockMutex.unlock();
		}
		_deltaTime = content->deltaTime;
		std::shared_ptr<Message> myMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::PhysicsReturnCall));
		MessagingSystem::instance().postMessage(myMessage);
		break;
	}
	case MESSAGE_TYPE::PhysicsAccelerateCallType: 
	{
		PhysicsAccelerateContent* content = static_cast<PhysicsAccelerateContent*>(myMessage->getContent());
		GameObject* go = content->object;
		GLfloat amount = content->amountFast;
		GLfloat amount2 = content->amountSlow;
		RigidBodyComponent *rbc = go->getComponent<RigidBodyComponent*>();
		
		if (amount != 0 && rbc->getAccNumber() < rbc->getMaxAcceleration())
		{
			rbc->setAccelerationVector(Vector3(rbc->getAccelerationVector()) + (Vector3(go->_transform._forward) * amount * _deltaTime));
			rbc->setAccNumber(amount * _deltaTime);
		}
		if (amount2 != 0)
		{
			if (rbc->getAccNumber() > 0) 
			{
				rbc->setAccelerationVector(Vector3(rbc->getAccelerationVector()) + (Vector3(go->_transform._forward) * 5.0f * -amount2 * _deltaTime));
				rbc->setAccNumber(-amount2 * 5.0f * _deltaTime);
				if (rbc->getAccNumber() <= 0) 
				{
					rbc->setAccNumber(-rbc->getAccNumber());
					rbc->setAccelerationVector(Vector3(0, 0, 0));
				}
			}
			if (rbc->getSpeed() > 0) 
			{
				rbc->setVelocity(Vector3(rbc->getVelocity()) + (Vector3(go->_transform._forward) * -amount2 * 5.0f * _deltaTime));
				rbc->setSpeed(-amount2 * 5.0f * _deltaTime);
				if (rbc->getSpeed() <= 0) 
				{
					rbc->setSpeed(-rbc->getSpeed());
					rbc->setVelocity(Vector3(0, 0, 0));
				}
			}
		}
		break;
	}
	default:
		break;
	}
}

void PhysicsEngine::generalPhysicsCall(GameObject* go) {
	if (go->hasComponent<RigidBodyComponent*>()) 
	{
		RigidBodyComponent* rbc = go->getComponent<RigidBodyComponent*>();
		applyAcceleration(go);
		//SDL_Log("%f, %f, %f", go->_transform._position.x, go->_transform._position.y, go->_transform._position.z);
		go->_transform.translate(Vector3(rbc->getVelocity()));
		//SDL_Log("Player Pos: %f", go->_transform._position.z);
	}
}

void PhysicsEngine::applyAcceleration(GameObject* go) {
	RigidBodyComponent* rc = go->getComponent<RigidBodyComponent*>();
	if (!(rc->getSpeed() > rc->getMaxVelocity()) || rc->getSpeed() < 0) {
		accelerate(go, rc);
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
void PhysicsEngine::accelerate(GameObject *go, RigidBodyComponent* rbc)
{
	rbc->setVelocity(Vector3(Vector3(rbc->getVelocity()) + (Vector3(rbc->getAccelerationVector()) * _deltaTime)));
	rbc->setSpeed(rbc->getAccNumber() * _deltaTime);
};
/**
 * <summary>
 * Accelerate the game object. Each axis should be modified by the delta time.
 * </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<RigidBodyComponent*>()->getVelocity() += Vector3(x, y, z) * _deltaTime;
	go->getComponent<RigidBodyComponent*>()->setSpeed(go->getComponent<RigidBodyComponent*>()->getAccNumber() * _deltaTime);
};
void PhysicsEngine::decelerate(GameObject *go, Vector3 amount)
{
	go->getComponent<RigidBodyComponent*>()->getVelocity() -= amount * _deltaTime;
};
void PhysicsEngine::decelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<RigidBodyComponent*>()->getVelocity() -= Vector3(x, y, z) * _deltaTime;
};
