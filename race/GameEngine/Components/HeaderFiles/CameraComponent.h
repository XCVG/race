#pragma once
#include "Component.h"
class CameraComponent : public Component
{
public:
	CameraComponent();
	CameraComponent(glm::vec3 _clearColor, GLfloat _nearPlane, GLfloat _farPlane, GLfloat _angle);
	CameraComponent(const CameraComponent &obj);
	void setClearColor(glm::vec3 _clearColor);
	void setNearPlane(GLfloat _nearPlane);
	void setFarPlane(GLfloat _farPlane);
	void setAngle(GLfloat _angle);
	glm::vec3 setClearColor();
	GLfloat setNearPlane();
	GLfloat setFarPlane();
	GLfloat setAngle();
private:
	glm::vec3 _clearColor;
	GLfloat _nearPlane;
	GLfloat _farPlane;
	GLfloat _angle;
};