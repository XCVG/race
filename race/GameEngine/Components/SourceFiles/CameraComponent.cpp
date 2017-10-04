#include "CameraComponent.h"
void CameraComponent::setClearColor(Vector3 _clearColor)
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
Vector3 CameraComponent::setClearColor()
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