#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <list>	
#include "Vector3.h"
#include "Component.h"
class GameObject
{
public:
	Vector3 _position;
	Vector3 _rotation;
	GLfloat _scale;
	std::list<Component> _components;
};