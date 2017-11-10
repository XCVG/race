#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif 
#include "Component.h"
#include "Quaternion.h"
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
	void setAccelerationVector(Vector3 _acceleration);
	void setMaxAcceleration(GLfloat _maxAcceleration);
	void setTurningDegree(GLfloat _num);
	void setForce(Vector3 _num);
	void setCenterOfGrav(Vector3 _num);
	GLfloat getWeight();
	GLfloat getBouncyness();
	GLfloat getFriction();
	GLfloat getResistance();
	Vector3 getAccelerationVector();
	GLfloat getMaxAcceleration();
	Vector3 getVelocity();
	GLfloat getMaxVelocity();
	GLfloat getTurningDegree();
	Vector3 getForce();
	Vector3 getCenterOfGrav();
private:
	GLfloat _weight;
	GLfloat _bouncyness;
	GLfloat _friction;
	GLfloat _resistance;
	Vector3 _acceleration;
	Vector3 _velocity;
	GLfloat _maxVelocity;
	GLfloat _turningDegree;
	Vector3 _force;
	glm::mat3x3 _mInertia;
	Vector3 _angularAccel;
	Vector3 _angularMoment;
	Quaternion _orientation;
};