#pragma once
#include <SDL_opengl.h>
#include "Component.h"
#include "Vector3.h"
class VelocityComponent : public Component
{
public:
	Vector3 _velocityVector;
	GLfloat _maxSpeed;
};