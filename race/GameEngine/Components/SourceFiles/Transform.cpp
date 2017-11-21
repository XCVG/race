#include "Transform.h"
Transform::Transform()
{
    this->_position = new Vector3();
    this->_rotation = new Vector3();
	this->_orientation.MakeQFromEulerAngles(this->_rotation.x, this->_rotation.y, this->_rotation.z);
    this->_scale = 1;
	this->_forward = Vector3(0, 0, 1);
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	adjustDirections(_rotation);
};
Transform::Transform(Vector3 _position, Vector3 _rotation, GLfloat _scale)
{
    this->_position = _position;
    this->_rotation = _rotation;
	this->_orientation.MakeQFromEulerAngles(this->_rotation.x, this->_rotation.y, this->_rotation.z);
    this->_scale = _scale;
	this->_forward = Vector3(0, 0, 1);
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	adjustDirections(this->_rotation);
};
Transform::Transform(Vector3 *_position, Vector3 *_rotation, GLfloat _scale)
{
    this->_position = *_position;
    this->_rotation = *_rotation;
	this->_orientation.MakeQFromEulerAngles(this->_rotation.x, this->_rotation.y, this->_rotation.z);
    this->_scale = _scale;
	this->_forward = Vector3(0, 0, 1);
	this->_right = Vector3(1, 0, 0);
	this->_up = Vector3(0, 1, 0);
	adjustDirections(this->_rotation);

};
Transform::Transform(const Transform &obj)
{
    this->_position = obj._position;
    this->_rotation = obj._rotation;
	this->_orientation.MakeQFromEulerAngles(this->_rotation.x, this->_rotation.y, this->_rotation.z);
    this->_scale = obj._scale;
	this->_forward = obj._forward;
	this->_right = obj._right;
	this->_up = obj._up;
	adjustDirections(this->_rotation);
};
void Transform::setPosition(Vector3 _position)
{
    this->_position = _position;
};
void Transform::setRotation(Vector3 _rotation)
{
    this->_rotation = _rotation;
	adjustDirections(this->_rotation);
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

void Transform::adjustDirections(Vector3 rot) 
{
	glm::mat4x4 matrix = glm::eulerAngleXYZ(rot.x, rot.y, rot.z);
	this->_forward = _forward.matrixMulti(matrix);
	this->_right = _right.matrixMulti(matrix);
	this->_up = _up.matrixMulti(matrix);
}

/**
* <summary>
* Rotate the object by a set amount. This rotation is in radians only,
* and is only ever increasing the rotation.
* Please specify positive/negative when calling. e.g., rotate(&go, -1.2);
* </summary>
*/
Vector3 Transform::rotateAround(Vector3 distance, Vector3 objectPos, Vector3 rotation) 
{
	Vector3 newDistance = distance.matrixMulti(glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z));
	this->_position = newDistance + objectPos;
	return newDistance;
};
void Transform::rotate(Vector3 amount)
{
	adjustDirections(-amount);
	//glm::mat4x4 matrix = glm::eulerAngleXYZ(amount.x, amount.y, amount.z);
	this->_rotation += amount;
	//SDL_Log("X:%f, Y:%f, Z:%f", _rotation.x, _rotation.y, _rotation.z);
};
void Transform::rotateQuat(Vector3 axis, GLfloat amount) {

	this->_orientation += this->_orientation * (axis * amount) * 0.5f;
	this->_orientation.Normalize();
};
void Transform::rotateX(GLfloat angle)
{
	adjustDirections(Vector3(angle, 0, 0));
	this->_rotation.x += angle;
};
void Transform::rotateY(GLfloat angle)
{
	adjustDirections(Vector3(0, angle, 0));
	this->_rotation.y += angle;
};
void Transform::rotateZ(GLfloat angle)
{
	adjustDirections(Vector3(0, 0, angle));
	this->_rotation.z += angle;
};

/**
*  <summary>
*  Move the game object in a direciton. The translation should be modified by the delta time.
*  </summary>
*/
void Transform::translate(Vector3 translation)
{
	this->_position += translation;
};

void Transform::translateForward(GLfloat num)
{
	this->_position += Vector3(_forward) * num;
};

void Transform::translateRight(GLfloat num) {
	this->_position += Vector3(_right) * num;
}
/**
*  <summary>
*  Move the game object in a direciton. Each axis should be modified by the delta time.
*  </summary>
*/
void Transform::translate(GLfloat x, GLfloat y, GLfloat z)
{
	this->_position += Vector3(x, y, z);
};