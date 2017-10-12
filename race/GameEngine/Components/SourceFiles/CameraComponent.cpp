#include "CameraComponent.h"
CameraComponent::CameraComponent()
{
	this->_clearColor = new glm::vec3();
	this->_nearPlane = 0;
	this->_farPlane = 0;
	this->_angle = 0;
};
CameraComponent::CameraComponent(glm::vec3 _clearColor, GLfloat _nearPlane, GLfloat _farPlane, GLfloat _angle)
{
	this->_clearColor = _clearColor;
	this->_nearPlane = _nearPlane;
	this->_farPlane = _farPlane;
	this->_angle = _angle;
};
CameraComponent::CameraComponent(const CameraComponent &obj)
{
	this->_clearColor = obj._clearColor;
	this->_nearPlane = obj._nearPlane;
	this->_farPlane = obj._farPlane;
	this->_angle = obj._angle;
};
void CameraComponent::setClearColor(glm::vec3 _clearColor)
{
    this->_clearColor = _clearColor;
};
void CameraComponent::setNearPlane(GLfloat _nearPlane)
{
    this->_nearPlane = _nearPlane;
};
void CameraComponent::setFarPlane(GLfloat _farPlane)
{
    this->_farPlane = _farPlane;
};
void CameraComponent::setAngle(GLfloat _angle)
{
    this->_angle = _angle;
};
glm::vec3 CameraComponent::setClearColor()
{
    return this->_clearColor;
};
GLfloat CameraComponent::setNearPlane()
{
    return this->_nearPlane;
};
GLfloat CameraComponent::setFarPlane()
{
    return this->_farPlane;
};
GLfloat CameraComponent::setAngle()
{
    return this->_angle;
};