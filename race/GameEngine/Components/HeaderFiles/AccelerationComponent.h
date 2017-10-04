#pragma once
#include <SDL_opengl.h>
#include "Component.h"
#include "Vector3.h"
class AccelerationComponent : public Component
{
public:
	void setAccelerationVector(Vector3 _acceleration);
	void setMaxAcceleration(GLfloat _maxAcceleration);
	Vector3 getAccelerationVector();
	GLfloat getMaxAcceleration();
private:
	Vector3 _acceleration;
	GLfloat _maxAcceleration;
};