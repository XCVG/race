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

void PhysicsEngine::checkMessage(std::shared_ptr<Message> myMessage) 
{

	switch (myMessage->getType()) {
	case MESSAGE_TYPE::PhysicsCallMessageType:
	{
		PhysicsCallMessageContent* content = static_cast<PhysicsCallMessageContent*>(myMessage->getContent());
		
		for (std::map<std::string, GameObject*>::iterator it = content->worldObjects.begin(); it != content->worldObjects.end(); ++it) {
			GameObject* go = it->second;
			generalPhysicsCall(go);
			
			if (it->first == "Sphere") {
				it->second->_transform.rotate(Vector3(0.0f, MATH_PI / 2, 0.0f) * content->deltaTime);
				//it->second->_transform._position += it->second->_transform._forward * content->deltaTime;
			}
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
		Vector3 F_Long;
		
		if (amount != 0 || amount2 != 0) {
			Vector3 F_Drag = Vector3(rbc->getVelocity() * rbc->getVelocity().magnitude()) * -0.4257;
			Vector3 F_RollingRes = rbc->getVelocity() * 12.8;
			Vector3 F_Long = F_Drag + F_RollingRes;
			if (amount != 0)
			{
				F_Long += go->_transform._forward.normalize() * (amount * 5000);
				rbc->setForce(F_Long);
			}
			if (amount2 != 0)
			{
				F_Long += Vector3(-go->_transform._forward.normalize()) * (amount2 * 6000);
				rbc->setForce(F_Long); 
			}
			rbc->setAccelerationVector(rbc->getForce() / rbc->getWeight());
		}
		rbc->setTurningDegree(content->turningDegree); // Turning input from user
		if (amount == 0 && amount2 == 0)
		{
			rbc->setAccelerationVector(Vector3(0, 0, 0));
			// rbc->setTurningDegree(0);
		}
		break;
	}
	default:
		break;
	}
}

void PhysicsEngine::generalPhysicsCall(GameObject* go) 
{
	if (go->hasComponent<RigidBodyComponent*>()) 
	{
		RigidBodyComponent* rbc = go->getComponent<RigidBodyComponent*>();
		applyAcceleration(go);
		applyTurning(go);
		//SDL_Log("%f, %f, %f", go->_transform._position.x, go->_transform._position.y, go->_transform._position.z);
		go->_transform.translate(Vector3(rbc->getVelocity()) * _deltaTime);
		//go->_transform.rotateY((MATH_PI / 2) * _deltaTime);
		//SDL_Log("Player Pos: %f", go->_transform._position.z);
	}
}

void PhysicsEngine::applyAcceleration(GameObject* go) 
{
	RigidBodyComponent* rc = go->getComponent<RigidBodyComponent*>();
	if (!(rc->getVelocity().magnitude() > rc->getMaxVelocity()) || rc->getVelocity().dotProduct(go->_transform._forward) < 0) 
	{
		accelerate(go, rc);
	}
}

void PhysicsEngine::applyTurning(GameObject* go)
{
	turnGameObject(go);
}

/**
 *	Stops the physics engine.
 */
void PhysicsEngine::stop()
{

	//SDL_Log("Physics::Stop");
}

void PhysicsEngine::flagLoop() 
{
	_running = false;
}

/**
 *  <summary>
 *  Accelerate the game object. The amount should be modified by the delta time.
 *  </summary>
 */
void PhysicsEngine::accelerate(GameObject *go, RigidBodyComponent* rbc)
{
	if (rbc->getVelocity().dotProduct(go->_transform._forward) >= 0) 
	{
		rbc->setVelocity(rbc->getVelocity() + (Vector3(rbc->getAccelerationVector()) * _deltaTime));
	}
	else 
	{
		rbc->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
		rbc->setForce(Vector3(0.0f, 0.0f, 0.0f));
	}
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
	if (tireDegree >= PI/4.0f || tireDegree != 0)
		SDL_Log("Joystick X");
	Vector3 objectVelocity = go->getComponent<RigidBodyComponent*>()->getVelocity(); 
	GLfloat L = (go->getChild(std::string("front"))->_transform._position 
		- go->getChild(std::string("rear"))->_transform._position).magnitude(); // Distance from front of object to rear of object
	GLfloat theta = tireDegree;
	if (L == 0 || theta == 0)
	{
		return Vector3();
	}
	GLfloat sinTheta = sin(theta);
	GLfloat denominator = (L / (sinTheta * 0.4));
	GLfloat omega = objectVelocity.magnitude() / denominator;
	return Vector3(0, omega, 0);
};

void PhysicsEngine::turnGameObject(GameObject *go)
{
	Vector3 angularVelocity = getAngleFromTurn(go, go->getComponent<RigidBodyComponent *>()->getTurningDegree());
	go->rotate(angularVelocity * (_deltaTime * 10)); // angularVelocity * deltaTime = current angle
};