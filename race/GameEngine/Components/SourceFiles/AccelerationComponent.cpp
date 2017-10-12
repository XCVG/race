#include "AccelerationComponent.h"
AccelerationComponent::AccelerationComponent()
{
    this->_acceleration = new Vector3();
    this->_maxAcceleration = 0;
};
AccelerationComponent::AccelerationComponent(glm::vec3 *_acceleration, GLfloat _maxAcceleration)
{
    this->_acceleration = *_acceleration;
    this->_maxAcceleration = _maxAcceleration;
};
AccelerationComponent::AccelerationComponent(glm::vec3 _acceleration, GLfloat _maxAcceleration)
{
    this->_acceleration = _acceleration;
    this->_maxAcceleration = _maxAcceleration;
};
AccelerationComponent::AccelerationComponent(const AccelerationComponent &obj)
{
    this->_acceleration = obj._acceleration;
    this->_maxAcceleration = obj._maxAcceleration;
};
void AccelerationComponent::setAccelerationVector(glm::vec3 _acceleration)
{
    this->_acceleration = _acceleration;
};
void AccelerationComponent::setMaxAcceleration(GLfloat _maxAcceleration)
{
    this->_maxAcceleration = _maxAcceleration;
};
glm::vec3 AccelerationComponent::getAccelerationVector()
{
    return this->_acceleration;
};
GLfloat AccelerationComponent::getMaxAcceleration()
{
    return this->_maxAcceleration;
};