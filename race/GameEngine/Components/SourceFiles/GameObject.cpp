#include "GameObject.h"
GameObject::GameObject()
{
    this->_position_p = new Vector3(0, 0, 0);
    this->_rotation_p = new Vector3(0, 0, 0);
    this->_scale = 1;
    this->_components_p = new std::map<std::string, Component *>();
};
GameObject::GameObject(Vector3 *_position, Vector3 *_rotation, GLfloat _scale)
{
    this->_position_p = _position;
    this->_rotation_p = _rotation;
    this->_scale = _scale;
	this->_components_p = new std::map<std::string, Component *>();
};
void GameObject::setPosition(Vector3 _position)
{
    this->_position_p = &_position;
};
void GameObject::setRotation(Vector3 _rotation)
{
    this->_rotation_p = &_rotation;
};
void GameObject::setScale(GLfloat _scale)
{
    this->_scale = _scale;
};
Vector3 GameObject::getPosition()
{
    return *this->_position_p;
};
Vector3 GameObject::getRotation()
{
    return *this->_rotation_p;
};
GLfloat GameObject::getScale()
{
    return this->_scale;
};
std::map<std::string, Component *> GameObject::getComponentList()
{
    return *this->_components_p;
};