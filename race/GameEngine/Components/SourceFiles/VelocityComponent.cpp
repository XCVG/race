#include "VelocityComponent.h"
void VelocityComponent::setVelocity(Vector3 _velocity)
{
    this->_velocity = _velocity;
};
void VelocityComponent::setMaxVelocity(GLfloat _maxVelocity)
{
    this->_maxVelocity = _maxVelocity;
};
Vector3 VelocityComponent::getVelocity()
{
    return this->_velocity;
};
GLfloat VelocityComponent::getMaxVelocity()
{
    return this->_maxVelocity;
};