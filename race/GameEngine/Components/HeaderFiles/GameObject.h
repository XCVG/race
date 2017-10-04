#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include <list>	
#include "Vector3.h"
#include "Component.h"
class GameObject
{
public:
	void setPosition(Vector3 _position);
	void setRotation(Vector3 _rotation);
	void setScale(GLfloat _scale);
	void addComponent(Component _component);
	void removeComponent(Component _component);
	Vector3 getPosition();
	Vector3 getRotation();
	GLfloat getScale();
	std::list<Component> getComponentList();
private:
	Vector3 _position;
	Vector3 _rotation;
	GLfloat _scale;
	std::list<Component> _components;
};