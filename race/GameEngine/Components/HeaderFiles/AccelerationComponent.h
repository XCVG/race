#pragma once
#include "Component.h"
class AccelerationComponent : public Component
{
public:
	AccelerationComponent();
	AccelerationComponent(glm::vec3 _acceleration, GLfloat _maxAcceleration);
	AccelerationComponent(glm::vec3 *_acceleration, GLfloat _maxAcceleration);
	AccelerationComponent(const AccelerationComponent &obj);
	void setAccelerationVector(glm::vec3 _acceleration);
	void setMaxAcceleration(GLfloat _maxAcceleration);
	glm::vec3 getAccelerationVector();
	GLfloat getMaxAcceleration();
private:
	glm::vec3 _acceleration;
	GLfloat _maxAcceleration;
};