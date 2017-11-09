#pragma once 
#ifdef __APPLE__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_opengl.h> 
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
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
	  Vector3 vec2 = Vector3();
	  vec2.x = this->x * mult;
	  vec2.y = this->y * mult;
	  vec2.z = this->z * mult;
	  return vec2;
  };
  Vector3& operator*(Vector3 vec)
  {
	  Vector3 vec2 = Vector3();
	  vec2.x = vec.x * this->x;
	  vec2.y = vec.y * this->y;
	  vec2.z = vec.z * this->z;
	  return vec2;
  }
  Vector3& operator/(GLfloat num) {
	  Vector3 vec2 = Vector3();
	  vec2.x = this->x / num;
	  vec2.y = this->y / num;
	  vec2.z = this->z / num;
	  return vec2;
  }
  Vector3& operator+(Vector3 vec) {
	  Vector3 vec2 = Vector3();
	  vec2.x = vec.x + this->x;
	  vec2.y = vec.y + this->y;
	  vec2.z = vec.z + this->z;
	  return vec2;
  }
  Vector3& operator-(Vector3 vec) {
	  this->x -= vec.x;
	  this->y -= vec.y;
	  this->z -= vec.z;
	  return *this;
  }
  Vector3& operator-() {
	  Vector3 vec2 = Vector3();
	  vec2.x = -this->x;
	  vec2.y = -this->y;
	  vec2.z = -this->z;
	  return vec2;
  }
  GLfloat magnitude()
  {
	  float x = powf(this->x, 2);
	  float y = powf(this->y, 2);
	  float z = powf(this->z, 2);
	  return (GLfloat)sqrtf(x + y + z);
  }
  Vector3& matrixMulti(glm::mat4x4 matrix) {
	  glm::vec4 temp = glm::vec4(this->x, this->y, this->z, 1) * matrix;
	  this->x = temp.x; this->y = temp.y, this->z = temp.z;
	  return *this;
  }
  Vector3& normalize() {
	  float num = sqrtf(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2));
	  this->x /= num; this->y /= num; this->z /= num;
	  return *this;
  }
  GLfloat dotProduct(Vector3 vec) {
	  return this->x * vec.x + this->y * vec.y + this->z * vec.z;
  }
  GLfloat x; 
  GLfloat y; 
  GLfloat z; 
};