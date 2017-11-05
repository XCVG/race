#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif 
#include "Component.h"
class RigidBodyComponent : public Component
{
public:
	RigidBodyComponent();
	RigidBodyComponent(GLfloat _maxAccel, GLfloat _maxVel, GLfloat _weight, GLfloat _bouncyness, GLfloat _friction, GLfloat _resistance);
	RigidBodyComponent(const RigidBodyComponent &obj);
	void setWeight(GLfloat _weight);
	void setBouncyness(GLfloat _bouncyness);
	void setFriction(GLfloat _friction);
	void setResistance(GLfloat _resistance);
	void setVelocity(Vector3 _velocity);
	void setMaxVelocity(GLfloat _maxVelocity);
	void setSpeed(GLfloat _speed);
	void setAccelerationVector(Vector3 _acceleration);
	void setMaxAcceleration(GLfloat _maxAcceleration);
	void setAccNumber(GLfloat _num);
	GLfloat getWeight();
	GLfloat getBouncyness();
	GLfloat getFriction();
	GLfloat getResistance();
	Vector3 getAccelerationVector();
	GLfloat getMaxAcceleration();
	GLfloat getAccNumber();
	Vector3 getVelocity();
	GLfloat getMaxVelocity();
	GLfloat getSpeed();
private:
	GLfloat _weight;
	GLfloat _bouncyness;
	GLfloat _friction;
	GLfloat _resistance;
	Vector3 _acceleration;
	GLfloat _maxAcceleration;
	GLfloat _accelerationNumber = 0;
	Vector3 _velocity;
	GLfloat _maxVelocity;
	GLfloat _speed;
};