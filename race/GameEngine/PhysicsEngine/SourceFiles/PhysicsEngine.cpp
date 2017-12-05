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
			F_Long = go->_transform._forward * (amount * 500);
		}
		if (amount2 != 0)
		{
			F_Long = -go->_transform._forward * (amount2 * 500);
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
		if (go->_name.compare("sphere") == 0) {
			rbc->setAngularAccel(Vector3(0, 0, PI) * 0.5 * _deltaTime);
		}
			
		if (go->_name.compare("player") == 0 && rbc->getVelocity().magnitude() >= 0)
		{
			adjustForces(go, rbc);
			applyAcceleration(go, rbc);
			go->translate(rbc->getVelocity() * _deltaTime);
			go->rotate(rbc->_angularVel * 0.5 * _deltaTime);
			turnGameObject(go);
		}
	}
};

void PhysicsEngine::applyAcceleration(GameObject *go, RigidBodyComponent *rc) 
{
	if (rc->getVelocity().magnitude() < rc->getMaxVelocity()) 
	{
		linearAccelerate(go,rc);
		//SDL_Log("SPEED: %f", rc->getVelocity().magnitude());
	}
	angularAccelerate(rc);
};

void PhysicsEngine::adjustForces(GameObject *go, RigidBodyComponent *rc) 
{
	Vector3 dragVector = -rc->getVelocity().normalize();
	Vector3 newForce = rc->getForce() + (dragVector * (rc->getVelocity().magnitude() *
		rc->getVelocity().magnitude()) * RHO * LINEARDRAGCOEF * ((rc->_length / 2) * (rc->_length / 2)));
	//newForce = QVRotate(go->_transform._orientation, newForce);

	rc->setAccelerationVector(newForce / rc->getMass());

	Vector3 angularDragVector = -rc->_angularVel.normalize();
	rc->_angularMoment += (angularDragVector * (rc->_angularVel.magnitude() *
		rc->_angularVel.magnitude()) * RHO * ANGULARDRAGCOEF * ((rc->_length / 2) * (rc->_length / 2)));

	glm::vec3 inertiaAngVel = rc->_mInertia * glm::vec3(rc->_angularVel.x, rc->_angularVel.y, rc->_angularVel.z);
	Vector3 angMoments = rc->_angularMoment - rc->_angularVel.crossProduct(Vector3(inertiaAngVel.x, inertiaAngVel.y, inertiaAngVel.z));
	glm::vec3 something = rc->_mInertiaInverse * glm::vec3(angMoments.x, angMoments.y, angMoments.z);
	rc->setAngularAccel(rc->getAngularAccel() + Vector3(something.x, something.y, something.z));
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
void PhysicsEngine::linearAccelerate(GameObject* go, RigidBodyComponent* rbc)
{
	rbc->setVelocity(go->_transform._forward * rbc->getVelocity().magnitude() + rbc->getAccelerationVector() * _deltaTime);
};


void PhysicsEngine::angularAccelerate(RigidBodyComponent* rbc) 
{
	//rbc->setAngularAccel();
	rbc->_angularVel += rbc->getAngularAccel() * _deltaTime;
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

Vector3 PhysicsEngine::getAngleFromTurn(GameObject *go, GLfloat tireDegree)
{
	Vector3 objectVelocity = go->getComponent<RigidBodyComponent*>()->getVelocity();
	Vector3 thing = go->getChild(std::string("forward"))->_transform._position - go->_transform._position;
	GLfloat L = (thing - (-thing)).magnitude();// Distance from front of object to rear of object
	GLfloat theta = tireDegree;
	if (L == 0 || theta == 0)
	{
		return Vector3(0, 0, 0);
	}
	GLfloat sinTheta = sin(theta);
	GLfloat denominator = (L / (sinTheta));
	Vector3 omega = go->_transform._forward.crossProduct(go->_transform._right.normalize()) / denominator;

	return omega;
};

void PhysicsEngine::turnGameObject(GameObject *go)
{
	//Vector3 angularVelocity = getAngleFromTurn(go, go->getComponent<RigidBodyComponent *>()->getTurningDegree());
	go->rotate(getAngleFromTurn(go, go->getComponent<RigidBodyComponent*>()->getTurningDegree()), 2.0 * _deltaTime); // angularVelocity * deltaTime = current angle
};