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
class CameraComponent : public Component
{
public:
	void setClearColor(Vector3 _clearColor);
	void setNearPlane(GLfloat _nearPlane);
	void setFarPlane(GLfloat _farPlane);
	void setAngle(GLfloat _angle);
	Vector3 setClearColor();
	GLfloat setNearPlane();
	GLfloat setFarPlane();
	GLfloat setAngle();
private:
	Vector3 _clearColor;
	GLfloat _nearPlane;
	GLfloat _farPlane;
	GLfloat _angle;
};