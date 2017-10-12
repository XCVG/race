#pragma once
#include <list>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include "ComponentHeaders.h"
class GameObject
{
public:
	GameObject();
	GameObject(glm::vec3 _position, glm::vec3 _rotation, GLfloat _scale);
	void setPosition(glm::vec3 _position);
	void setRotation(glm::vec3 _rotation);
	void setScale(GLfloat _scale);
	void addComponent(Component *_component_p);
	void removeComponent(Component _component);
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	GLfloat getScale();
	std::vector<Component *> getComponentList();
	template <class Comp>
	Component* getComponent(Comp type);
private:
	glm::vec3 *_position_p;
	glm::vec3 *_rotation_p;
	GLfloat _scale;
	std::vector<Component *> *_components_p; // TODO: Change to vector
};