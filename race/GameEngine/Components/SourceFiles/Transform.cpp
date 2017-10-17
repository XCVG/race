#include "Transform.h"
Transform::Transform()
{
    this->_position = new Vector3();
    this->_rotation = new Vector3();
    this->_scale = 1;
};
Transform::Transform(Vector3 _position, Vector3 _rotation, GLfloat _scale)
{
    this->_position = _position;
    this->_rotation = _rotation;
    this->_scale = _scale;
};
Transform::Transform(Vector3 *_position, Vector3 *_rotation, GLfloat _scale)
{
    this->_position = *_position;
    this->_rotation = *_rotation;
    this->_scale = _scale;
};
Transform::Transform(const Transform &obj)
{
    this->_position = obj._position;
    this->_rotation = obj._rotation;
    this->_scale = obj._scale;
};
void Transform::setPosition(Vector3 _position)
{
    this->_position = _position;
};
void Transform::setRotation(Vector3 _rotation)
{
    this->_rotation = _rotation;
};
void Transform::setScale(GLfloat _scale)
{
    this->_scale = _scale;
};
Vector3 Transform::getPosition()
{
    return this->_position;
};
Vector3 Transform::getRotation()
{
    return this->_rotation;
};
GLfloat Transform::getScale()
{
    return this->_scale;
};
Vector3 Transform::getForward()
{
	return this->_forward;
};