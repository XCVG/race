#pragma once
#include <SDL_opengl.h>
#include "Component.h"
#include "Vector3.h"
class CameraComponent : public Component
{
public:
	Vector3 _clearColor;
	GLfloat _nearPlane;
	GLfloat _farPlane;
	GLfloat _angle;
};