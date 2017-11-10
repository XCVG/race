#include "RigidBodyComponent.h"
RigidBodyComponent::RigidBodyComponent()
{
    this->_weight = 0;
    this->_bouncyness = 0;
    this->_friction = 0;
    this->_resistance = 0;
	this->_maxVelocity = 0;
	this->_turningDegree = 0;
	this->_velocity = new Vector3();
	this->_acceleration = new Vector3();
};
RigidBodyComponent::RigidBodyComponent(GLfloat _maxAccel, GLfloat _maxVel, GLfloat _weight, GLfloat _bouncyness, GLfloat _friction, GLfloat _resistance)
{
    this->_weight = _weight;
    this->_bouncyness = _bouncyness;
    this->_friction = _friction;
    this->_resistance = _resistance;
	this->_maxVelocity = _maxVel;
	this->_turningDegree = 0;
	this->_velocity = new Vector3();
	this->_acceleration = new Vector3();
};
RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent &obj)
{
    this->_weight = obj._weight;
    this->_bouncyness = obj._bouncyness;
    this->_friction = obj._friction;
    this->_resistance = obj._resistance;
	this->_maxVelocity = obj._maxVelocity;
	this->_turningDegree = obj._turningDegree;
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
void RigidBodyComponent::setTurningDegree(GLfloat _num) 
{
	this->_turningDegree = _num;
};
void RigidBodyComponent::setForce(Vector3 _num) 
{
	this->_force = _num;
};
Vector3 RigidBodyComponent::getAccelerationVector()
{
	return this->_acceleration;
};
void RigidBodyComponent::setVelocity(Vector3 _velocity)
{
	this->_velocity = _velocity;
};
void RigidBodyComponent::setMaxVelocity(GLfloat _maxVelocity)
{
	this->_maxVelocity = _maxVelocity;
};
Vector3 RigidBodyComponent::getVelocity()
{
	return this->_velocity;
};
GLfloat RigidBodyComponent::getMaxVelocity()
{
	return this->_maxVelocity;
};
GLfloat RigidBodyComponent::getTurningDegree() 
{
	return this->_turningDegree;
};
Vector3 RigidBodyComponent::getForce() 
{
	return this->_force;
};