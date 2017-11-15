#pragma once
#include <list>
//#include <vector>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <mutex>
#include "Transform.h"
#include "ComponentHeaders.h"
class GameObject
{
public:
	GameObject();
	GameObject(Transform transform);
	GameObject(Transform *transform);
	Transform _transform;
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
	void addChild(GameObject *child);
	GameObject* getChild(GameObject *child);
	void removeChild(GameObject *child);
	std::vector<GameObject *> getChildObjectList();
	bool operator==(const GameObject& go) const;
private:
	std::map<std::string, Component *> *_components_p;
	std::vector<GameObject *> *_childObjects_p;
	template <typename T>
	std::string getType()
	{
		return typeid(T).name();
	};
};
inline bool GameObject::operator==(const GameObject& go) const
{
	if (!this->_transform == go._transform)
	{
		return false;
	}
	// Check each component // REVIEW: How do I write this??
	/**
	if (this->getComponentList() != NULL && go.getComponentList() != NULL)
	{
		bool compEqual;
		for (Component *comp : this->getComponentList())
		{
			compEqaul = false;
			for (Component *comp2 : this->getComponentList())
			{
				if (*comp == *comp2)
				{
					compEqual = true;
				}
			}
			if (!compEqual)
			{
				return false;
			}
		}
		if (!compEqual)
		{
			return false;
		}
	}
	*/
	// Check each child
	if (this->getComponentList() != NULL && go.getComponentList() != NULL)
	{
		bool childEqual;
		for (GameObject *child : this->getChildObjectList())
		{
			childEqual = false;
			for (GameObject *child2 : go.getChildObjectList())
			{
				if (*child == *child2)
				{
					childEqual = true;
				}
			}
			if (!childEqual)
			{
				return false; // No similar children found
			}
		}
		if (!childEqual)
		{
			return false;
		}
	}
	return true;
};