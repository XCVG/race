#include "GameObject.h"
GameObject::GameObject()
{
    this->_position_p = new glm::vec3(0, 0, 0);
    this->_rotation_p = new glm::vec3(0, 0, 0);
    this->_scale = 1;
    this->_components_p = new std::vector<Component *>();
};
GameObject::GameObject(glm::vec3 _position, glm::vec3 _rotation, GLfloat _scale)
{
    this->_position_p = &_position;
    this->_rotation_p = &_rotation;
    this->_scale = _scale;
};
void GameObject::setPosition(glm::vec3 _position)
{
    this->_position_p = &_position;
};
void GameObject::setRotation(glm::vec3 _rotation)
{
    this->_rotation_p = &_rotation;
};
void GameObject::setScale(GLfloat _scale)
{
    this->_scale = _scale;
};
glm::vec3 GameObject::getPosition()
{
    return *this->_position_p;
};
glm::vec3 GameObject::getRotation()
{
    return *this->_rotation_p;
};
GLfloat GameObject::getScale()
{
    return this->_scale;
};
std::vector<Component *> GameObject::getComponentList()
{
    return *this->_components_p;
};