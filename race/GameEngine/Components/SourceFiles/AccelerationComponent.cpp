#include "AccelerationComponent.h"
void AccelerationComponent::setAccelerationVector(Vector3 _acceleration)
{
    this->_acceleration = _acceleration;
};
void AccelerationComponent::setMaxAcceleration(GLfloat _maxAcceleration)
{
    this->_maxAcceleration = _maxAcceleration;
};
Vector3 AccelerationComponent::getAccelerationVector()
{
    return this->_acceleration;
};
GLfloat AccelerationComponent::getMaxAcceleration()
{
    return this->_maxAcceleration;
};