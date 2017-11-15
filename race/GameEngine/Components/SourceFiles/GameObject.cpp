#include "GameObject.h"
GameObject::GameObject()
{
    this->_transform = Transform();
	this->_name = "";
    this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(std::string name)
{
    this->_transform = Transform();
	this->_name = name;
    this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(Transform transform)
{
	this->_transform = transform;
	this->_name = "";
	this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(Transform *transform)
{
	this->_transform = *transform;
	this->_name = "";
	this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(Transform transform, std::string name)
{
	this->_transform = transform;
	this->_name = name;
	this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(Transform *transform, std::string name)
{
	this->_transform = *transform;
	this->_name = name;
	this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
void GameObject::setName(std::string name)
{
	this->_name = name;
};
std::string GameObject::getName()
{
	return this->_name;
};
std::map<std::string, Component *> GameObject::getComponentList()
{
    return *this->_components_p;
};
void GameObject::addChild(GameObject *child)
{
	this->_childObjects_p->push_back(child);
};
GameObject* GameObject::getChild(GameObject *child)
{
	for (auto &go : *(this->_childObjects_p))
	{
		if (*go == *child)
		{
			return go;
		}
	}
	return nullptr;
};
GameObject* GameObject::getChild(std::string name)
{
	for (auto &go : *(this->_childObjects_p))
	{
		if (go->_name == name)
		{
			return go;
		}
	}
	return nullptr;
};
void GameObject::removeChild(GameObject *child)
{
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
			i != this->_childObjects_p->end();)
	{
		if (**i == *child)
		{
			i = this->_childObjects_p->erase(i);
		}
		else
		{
			++i;
		}
	}
};
std::vector<GameObject *> GameObject::getChildObjectList()
{
	return *this->_childObjects_p;
};