#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include <list>
#include <vector>
#include "Vector3.h"
#include "Component.h"
class GameObject
{
public:
	GameObject();
	GameObject(Vector3 _position, Vector3 _rotation, GLfloat _scale);
	void setPosition(Vector3 _position);
	void setRotation(Vector3 _rotation);
	void setScale(GLfloat _scale);
	void addComponent(Component _component);
	void removeComponent(Component _component);
	Vector3 getPosition();
	Vector3 getRotation();
	GLfloat getScale();
	std::vector<Component> getComponentList();
private:
	Vector3 *_position_p;
	Vector3 *_rotation_p;
	GLfloat _scale;
	std::vector<Component> *_components_p; // TODO: Change to vector
};