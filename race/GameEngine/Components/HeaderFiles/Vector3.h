#pragma once 
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h> 
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h> 
#endif 
class Vector3 
{ 
public: 
  Vector3(); 
  Vector3(GLfloat x, GLfloat y, GLfloat z); 
  Vector3(const Vector3 &obj); 
  Vector3& operator=(const Vector3* obj)  
  { 
    this->x = obj->x; 
    this->y = obj->y; 
    this->z = obj->z; 
    return *this; 
  }; 
  Vector3& operator=(const Vector3& obj)  
  { 
    this->x = obj.x; 
    this->y = obj.y; 
    this->z = obj.z; 
    return *this; 
  };
  Vector3& operator+=(const Vector3* obj)
  {
    this->x = this->x + obj->x;
    this->y = this->y + obj->y;
    this->z = this->z + obj->z;
	return *this;
  };
  Vector3& operator+=(const Vector3& obj)
  {
    this->x = this->x + obj.x;
    this->y = this->y + obj.y;
    this->z = this->z + obj.z;
	return *this;
  };
  Vector3& operator-=(const Vector3* obj)
  {
    this->x = this->x - obj->x;
    this->y = this->y - obj->y;
    this->z = this->z - obj->z;
	return *this;
  };
  Vector3& operator-=(const Vector3& obj)
  {
    this->x = this->x - obj.x;
    this->y = this->y - obj.y;
    this->z = this->z - obj.z;
	return *this;
  };
  Vector3& operator*(const float_t& mult)
  {
	  this->x = this->x * mult;
	  this->y = this->y * mult;
	  this->z = this->z * mult;
	  return *this;
  };
  GLfloat x; 
  GLfloat y; 
  GLfloat z; 
};