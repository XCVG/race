#pragma once
#include "Component.h"
class VelocityComponent : public Component
{
public:
	VelocityComponent();
	VelocityComponent(glm::vec3 _velocity, GLfloat _maxVelocity);
	VelocityComponent(glm::vec3 *_velocity, GLfloat _maxVelocity);
	VelocityComponent(const VelocityComponent &obj);
	void setVelocity(glm::vec3 _velocity);
	void setMaxVelocity(GLfloat _maxVelocity);
	glm::vec3 getVelocity();
	GLfloat getMaxVelocity();
private:
	glm::vec3 _velocity;
	GLfloat _maxVelocity;
};