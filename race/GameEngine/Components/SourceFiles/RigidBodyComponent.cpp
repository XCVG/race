#include "RigidBodyComponent.h"
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