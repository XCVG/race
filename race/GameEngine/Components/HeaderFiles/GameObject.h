#pragma once
#include <list>
//#include <vector>
#include <map>
#include <typeinfo>
#include <typeindex>
#include "ComponentHeaders.h"
class GameObject
{
public:
	GameObject();
	GameObject(Vector3 _position, Vector3 _rotation, GLfloat _scale);
	void setPosition(Vector3 _position);
	void setRotation(Vector3 _rotation);
	void setScale(GLfloat _scale);
	template <typename T>
	void addComponent(T component)
	{
		std::string type = this->getType<T>();
		(*this->_components_p)[type] = component;
	};
	template <typename T>
	void removeComponent()
	{
		std::string type = getType<T>();
		if (this->hasComponent<T>())
		{
			this->_components_p->erase(this->_components_p->find(type));
		}
	};
	Vector3 getPosition();
	Vector3 getRotation();
	GLfloat getScale();
	template <typename T>
	T getComponent()
	{
		if (this->hasComponent<T>())
		{
			return (T)(*this->_components_p)[this->getType<T>()]; // TODO: Dynamic cast?
		}
		else
		{
			return NULL;
		}
	};
	template <typename T>
	bool hasComponent()
	{
		if (this->_components_p->find(this->getType<T>()) == this->_components_p->end())
		{
			return false;
		}
		else
		{
			return true;
		}
	};
	std::map<std::string, Component *> getComponentList();
private:
	Vector3 *_position_p;
	Vector3 *_rotation_p;
	GLfloat _scale;
	std::map<std::string, Component *> *_components_p;
	template <typename T>
	std::string getType()
	{
		return typeid(T).name();
	};
	
};