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
void GameObject::rotate(Vector3 vec, GLfloat angle)
{
	this->_transform.rotateQuat(vec, angle);
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
		i != this->_childObjects_p->end();)
	{
		Quaternion q;
		(*i)->_transform._orientation = this->_transform._orientation * q.MakeQFromEulerAngles((*i)->_transform._rotation);
		if ((*i)->_name == "up")
			(*i)->_transform._position = this->_transform._position + this->_transform._up;
		else if ((*i)->_name == "forward") 
			(*i)->_transform._position = this->_transform._position + this->_transform._forward;
		else if ((*i)->_name == "right")
			(*i)->_transform._position = this->_transform._position + this->_transform._right;
		
		i++;
	}
};
void GameObject::rotateX(GLfloat angle) 
{
	this->_transform.rotateX(angle);
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
		i != this->_childObjects_p->end();)
	{
		(*i)->_transform.rotateX(angle);
		i++;
	}

};
void GameObject::rotateY(GLfloat angle)
{
	this->_transform.rotateY(angle);
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
		i != this->_childObjects_p->end();)
	{
		(*i)->_transform.rotateY(angle);
		i++;
	}
};
void GameObject::rotateZ(GLfloat angle)
{
	this->_transform.rotateZ(angle);
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
		i != this->_childObjects_p->end();)
	{
		(*i)->_transform.rotateZ(angle);
		i++;
	}
};