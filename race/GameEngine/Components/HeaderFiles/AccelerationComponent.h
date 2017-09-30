#pragma once
#include <SDL_opengl.h>
#include "Component.h"
#include "Vector3.h"
class AccelerationComponent : public Component
{
public:
	Vector3 _accelerationVector;
	GLfloat _maxAcceleration;
};