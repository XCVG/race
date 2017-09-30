#pragma once
#include <SDL_opengl.h>
#include "Component.h"
class LightComponent : public Component
{
public:
	GLfloat _intensity;
	// type
	GLfloat _range;
	GLfloat _angle;
};