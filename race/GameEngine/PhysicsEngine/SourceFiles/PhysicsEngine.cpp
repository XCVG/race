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
};

void PhysicsEngine::checkMessage(std::shared_ptr<Message> myMessage) 
{

	switch (myMessage->getType()) {
	case MESSAGE_TYPE::PhysicsCallMessageType:
	{
		PhysicsCallMessageContent* content = static_cast<PhysicsCallMessageContent*>(myMessage->getContent());
		_deltaTime = content->deltaTime;
		for (std::map<std::string, GameObject*>::iterator it = content->worldObjects.begin(); it != content->worldObjects.end(); ++it) {
			GameObject* go = it->second;
			generalPhysicsCall(go);
			
			if (it->first.compare("Cube") == 0) {
				it->second->rotate(Vector3(1.0f, 0.0f, 0.0f), PI * _deltaTime * 0.5f);
				//it->second->_transform.translateForward(1.0f * _deltaTime);
			}
		}
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
		Vector3 F_Long;
		if (amount != 0)
		{
			F_Long += go->_transform._forward.normalize() * (amount * 500);
		}
		if (amount2 != 0)
		{
			F_Long += -go->_transform._forward.normalize() * (amount2 * 500);
		}
		rbc->setForce(F_Long);
		rbc->setTurningDegree(content->turningDegree); // Turning input from user
		break;
	}
	default:
		break;
	}
};

void PhysicsEngine::generalPhysicsCall(GameObject* go) 
{
	if (go->hasComponent<RigidBodyComponent*>()) 
	{
		RigidBodyComponent* rbc = go->getComponent<RigidBodyComponent*>();
		if (rbc->getVelocity().dotProduct(go->_transform._forward) >= 0) {
			adjustForces(rbc);
			applyAcceleration(rbc);
			go->translate(Vector3(rbc->getVelocity()) * _deltaTime);
		}
		else {
			rbc->setVelocity(Vector3());
			rbc->setForce(Vector3());
		}
	}
};

void PhysicsEngine::applyAcceleration(RigidBodyComponent *rc) 
{
	if (rc->getVelocity().magnitude() < rc->getMaxVelocity()) 
	{
		linearAccelerate(rc);
		angularAccelerate(rc);
	}
};

void PhysicsEngine::adjustForces(RigidBodyComponent *rc) 
{
	Vector3 dragVector = -rc->getVelocity().normalize();
	Vector3 newForce = rc->getForce() + (dragVector * (rc->getVelocity().magnitude() *
		rc->getVelocity().magnitude()) * RHO * LINEARDRAGCOEF * ((rc->_length / 2) * (rc->_length / 2)));

	rc->setAccelerationVector(newForce / rc->getMass());
	rc->setForce(newForce);
}


void PhysicsEngine::applyTurning(GameObject* go)
{
	turnGameObject(go);
};

/**
 *	Stops the physics engine.
 */
void PhysicsEngine::stop() {};

void PhysicsEngine::flagLoop() 
{
	_running = false;
};

/**
 *  <summary>
 *  Accelerate the game object. The amount should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::linearAccelerate(RigidBodyComponent* rbc)
{
	rbc->setVelocity(rbc->getVelocity() + rbc->getAccelerationVector() * _deltaTime);
};


void PhysicsEngine::angularAccelerate(RigidBodyComponent* rbc) 
{
	//rbc->setAngularAccel();
};

/**
 * <summary>
 * Accelerate the game object. Each axis should be modified by the delta time.
 * </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	//go->getComponent<RigidBodyComponent*>()->getVelocity() += Vector3(x, y, z) * _deltaTime;
	//go->getComponent<RigidBodyComponent*>()->setSpeed(go->getComponent<RigidBodyComponent*>()->getAccNumber() * _deltaTime);
};

void PhysicsEngine::decelerate(GameObject *go, GLfloat x, GLfloat y, GLfloat z)
{
	go->getComponent<RigidBodyComponent*>()->getVelocity() -= Vector3(x, y, z) * _deltaTime;
};

Vector3 PhysicsEngine::getAngleFromTurn(GameObject *go, GLfloat tireDegree)
{
	/*Vector3 objectVelocity = go->getComponent<RigidBodyComponent*>()->getVelocity(); 
	GLfloat L = (go->getChild(std::string("forward"))->_transform._position 
		- (-go->getChild(std::string("forward"))->_transform._position)).magnitude(); // Distance from front of object to rear of object
	GLfloat theta = tireDegree;
	if (L == 0 || theta == 0)
	{
		return 0;
	}
	GLfloat sinTheta = sin(theta);
	GLfloat denominator = (L / (sinTheta));
	GLfloat omega = objectVelocity.magnitude() / denominator;
	return omega;*/


	return Vector3();
};

void PhysicsEngine::turnGameObject(GameObject *go)
{
	//GLfloat angularVelocity = getAngleFromTurn(go, go->getComponent<RigidBodyComponent *>()->getTurningDegree());
	go->rotate(getAngleFromTurn(go, go->getComponent<RigidBodyComponent*>()->getTurningDegree()), 0.5 * (_deltaTime)); // angularVelocity * deltaTime = current angle
};