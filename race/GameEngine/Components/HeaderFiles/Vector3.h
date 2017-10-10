#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
class Vector3
{
public:
	Vector3();
	Vector3(GLfloat x, GLfloat y, GLfloat z);
	Vector3(const Vector3 &obj);
	Vector3& operator=(const Vector3* obj) 
	{
		this->_x = obj->_x;
		this->_y = obj->_y;
		this->_z = obj->_x;
		return *this;
	};
	Vector3& operator=(const Vector3& obj) 
	{
		this->_x = obj._x;
		this->_y = obj._y;
		this->_z = obj._x;
		return *this;
	};
	GLfloat _x;
	GLfloat _y;
	GLfloat _z;
};