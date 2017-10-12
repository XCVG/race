#include "VelocityComponent.h"
VelocityComponent::VelocityComponent()
{
    this->_velocity = new glm::vec3();
    this->_maxVelocity = 0;
};
VelocityComponent::VelocityComponent(glm::vec3 _velocity, GLfloat _maxVelocity)
{
    this->_velocity = _velocity;
    this->_maxVelocity = _maxVelocity;
};
VelocityComponent::VelocityComponent(glm::vec3 *_velocity, GLfloat _maxVelocity)
{
    this->_velocity = *_velocity;
    this->_maxVelocity = _maxVelocity;
};
VelocityComponent::VelocityComponent(const VelocityComponent &obj)
{
    this->_velocity = obj._velocity;
    this->_maxVelocity = obj._maxVelocity;
};
void VelocityComponent::setVelocity(glm::vec3 _velocity)
{
    this->_velocity = _velocity;
};
void VelocityComponent::setMaxVelocity(GLfloat _maxVelocity)
{
    this->_maxVelocity = _maxVelocity;
};
glm::vec3 VelocityComponent::getVelocity()
{
    return this->_velocity;
};
GLfloat VelocityComponent::getMaxVelocity()
{
    return this->_maxVelocity;
};