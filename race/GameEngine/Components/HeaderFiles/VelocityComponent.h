#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include "Component.h"
#include "Vector3.h"
class VelocityComponent : public Component
{
public:
	VelocityComponent();
	VelocityComponent(Vector3 _velocity, GLfloat _maxVelocity);
	VelocityComponent(Vector3 *_velocity, GLfloat _maxVelocity);
	VelocityComponent(const VelocityComponent &obj);
	void setVelocity(Vector3 _velocity);
	void setMaxVelocity(GLfloat _maxVelocity);
	Vector3 getVelocity();
	GLfloat getMaxVelocity();
private:
	Vector3 _velocity;
	GLfloat _maxVelocity;
};