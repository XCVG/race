#include "GameObject.h"
GameObject::GameObject()
{
    this->_transform = Transform();
    this->_components_p = new std::map<std::string, Component *>();
};
GameObject::GameObject(Transform transform)
{
	this->_transform = transform;
	this->_components_p = new std::map<std::string, Component *>();
};
GameObject::GameObject(Transform *transform)
{
	this->_transform = *transform;
	this->_components_p = new std::map<std::string, Component *>();
};
std::map<std::string, Component *> GameObject::getComponentList()
{
    return *this->_components_p;
};
void GameObject::addChild(GameObject *child)
{
	this->_childObjects_p->add(child); // DEBUG: Test this
};
GameObject* GameObject::getChild(GameObject *child)
{
	this->_childObjects_p->find(child); // DEBUG: Test this
};
void GameObject::removeChild(GameObject *child)
{
	this->_childObjects_p->remove(this->_childObjects_p.find(child));
};
std::vector<GameObject *> GameObject::getChildObjectList()
{
	return *this->_childObjects_p;
};