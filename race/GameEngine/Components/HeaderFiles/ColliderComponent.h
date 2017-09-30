#pragma once
#include "Component.h"
#include <SDL_opengl.h>
#include "Vector3.h"
class ColliderComponent : public Component
{
public:
	Vector3 _verticies;
};