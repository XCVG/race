#include "RigidBodyComponent.h"
RigidBodyComponent::RigidBodyComponent()
{
    this->_weight = 0;
    this->_bouncyness = 0;
    this->_friction = 0;
    this->_resistance = 0;
	this->_maxAcceleration = 0;
	this->_maxVelocity = 0;
	this->_speed = 0;
	this->_accelerationNumber = 0;
	this->_velocity = new Vector3();
	this->_acceleration = new Vector3();
};
RigidBodyComponent::RigidBodyComponent(GLfloat _maxAccel, GLfloat _maxVel, GLfloat _weight, GLfloat _bouncyness, GLfloat _friction, GLfloat _resistance)
{
    this->_weight = _weight;
    this->_bouncyness = _bouncyness;
    this->_friction = _friction;
    this->_resistance = _resistance;
	this->_maxAcceleration = _maxAccel;
	this->_maxVelocity = _maxVel;
	this->_speed = 0;
	this->_accelerationNumber = 0;
	this->_velocity = new Vector3();
	this->_acceleration = new Vector3();
};
RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent &obj)
{
    this->_weight = obj._weight;
    this->_bouncyness = obj._bouncyness;
    this->_friction = obj._friction;
    this->_resistance = obj._resistance;
	this->_maxAcceleration = obj._maxAcceleration;
	this->_maxVelocity = obj._maxVelocity;
	this->_speed = obj._speed;
	this->_accelerationNumber = obj._accelerationNumber;
	this->_velocity = obj._velocity;
	this->_acceleration = obj._acceleration;

};
void RigidBodyComponent::setWeight(GLfloat _weight)
{
    this->_weight = _weight;
};
void RigidBodyComponent::setBouncyness(GLfloat _bouncyness)
{
    this->_bouncyness = _bouncyness;
};
void RigidBodyComponent::setFriction(GLfloat _friction)
{
    this->_friction = _friction;
};
void RigidBodyComponent::setResistance(GLfloat _resistance)
{
    this->_resistance = _resistance;
};
GLfloat RigidBodyComponent::getWeight()
{
    return this->_weight;
};
GLfloat RigidBodyComponent::getBouncyness()
{
    return this->_bouncyness;
};
GLfloat RigidBodyComponent::getFriction()
{
    return this->_friction;
};
GLfloat RigidBodyComponent::getResistance()
{
    return this->_resistance;
};
void RigidBodyComponent::setAccelerationVector(Vector3 _acceleration)
{
	this->_acceleration = _acceleration;
};
void RigidBodyComponent::setMaxAcceleration(GLfloat _maxAcceleration)
{
	this->_maxAcceleration = _maxAcceleration;
};
void RigidBodyComponent::setAccNumber(GLfloat _num) 
{
	this->_accelerationNumber += _num;
}
Vector3 RigidBodyComponent::getAccelerationVector()
{
	return this->_acceleration;
};
GLfloat RigidBodyComponent::getMaxAcceleration()
{
	return this->_maxAcceleration;
};
GLfloat RigidBodyComponent::getAccNumber()
{
	return this->_accelerationNumber;
}
void RigidBodyComponent::setVelocity(Vector3 _velocity)
{
	this->_velocity = _velocity;
};
void RigidBodyComponent::setMaxVelocity(GLfloat _maxVelocity)
{
	this->_maxVelocity = _maxVelocity;
};
void RigidBodyComponent::setSpeed(GLfloat _speed)
{
	this->_speed += _speed;
}
Vector3 RigidBodyComponent::getVelocity()
{
	return this->_velocity;
};
GLfloat RigidBodyComponent::getMaxVelocity()
{
	return this->_maxVelocity;
};
GLfloat RigidBodyComponent::getSpeed()
{
	return this->_speed;
}