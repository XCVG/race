#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include "Component.h"
class LightComponent : public Component
{
public:
	GLfloat _intensity;
	// type
	GLfloat _range;
	GLfloat _angle;
};