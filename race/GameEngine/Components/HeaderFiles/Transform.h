#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include "Vector3.h"
class Transform
{
public:
    Transform();
    Transform(Vector3 _position, Vector3 _rotation, GLfloat _scale);
    Transform(Vector3 *_position, Vector3 *_rotation, GLfloat _scale);
    Transform(const Transform &obj);
    void setPosition(Vector3 _position);
	void setRotation(Vector3 _rotation);
    void setScale(GLfloat _scale);
    Vector3 getPosition();
	Vector3 getRotation();
	GLfloat getScale();
	Vector3 getForward();
    Vector3 _position;
    Vector3 _rotation;
    GLfloat _scale;
	Vector3 _forward = Vector3(0, 0, 1);
	Vector3 _right = Vector3(1, 0, 0);
	Vector3 _up = Vector3(0, 1, 0);
private:
};