#include "Transform.h"
Transform::Transform()
{
    this->_position = new Vector3();
    this->_rotation = new Vector3();
	_orientation.MakeQFromEulerAngles(this->_rotation.x, this->_rotation.y, this->_rotation.z);
    this->_scale = 1;
	_forward = Vector3(0, 0, 1);
	_right = Vector3(1, 0, 0);
	_up = Vector3(0, 1, 0);
	adjustDirections(_rotation);
};
Transform::Transform(Vector3 _position, Vector3 _rotation, GLfloat _scale)
{
    this->_position = _position;
    this->_rotation = _rotation;
	_orientation.MakeQFromEulerAngles(this->_rotation.x, this->_rotation.y, this->_rotation.z);
    this->_scale = _scale;
	_forward = Vector3(0, 0, 1);
	_right = Vector3(1, 0, 0);
	_up = Vector3(0, 1, 0);
	adjustDirections(this->_rotation);
};
Transform::Transform(Vector3 *_position, Vector3 *_rotation, GLfloat _scale)
{
    this->_position = *_position;
    this->_rotation = *_rotation;
	_orientation.MakeQFromEulerAngles(this->_rotation.x, this->_rotation.y, this->_rotation.z);
    this->_scale = _scale;
	_forward = Vector3(0, 0, 1);
	_right = Vector3(1, 0, 0);
	_up = Vector3(0, 1, 0);
	adjustDirections(this->_rotation);

};
Transform::Transform(const Transform &obj)
{
    this->_position = obj._position;
    this->_rotation = obj._rotation;
	_orientation.MakeQFromEulerAngles(this->_rotation.x, this->_rotation.y, this->_rotation.z);
    this->_scale = obj._scale;
	_forward = obj._forward;
	_right = obj._right;
	_up = obj._up;
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
	_forward = _forward.matrixMulti(matrix);
	_right = _right.matrixMulti(matrix);
	_up = _up.matrixMulti(matrix);
}

/**
* <summary>
* Rotate the object by a set amount. This rotation is in radians only,
* and is only ever increasing the rotation.
* Please specify positive/negative when calling. e.g., rotate(&go, -1.2);
* </summary>
*/
void Transform::rotateAround(Vector3 objectPos, Vector3 rotation) 
{
	Vector3 distance = this->_position - objectPos;
	this->_position = distance.matrixMulti(glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z)) + objectPos;
};
void Transform::rotate(Vector3 amount)
{
	adjustDirections(amount);
	//glm::mat4x4 matrix = glm::eulerAngleXYZ(amount.x, amount.y, amount.z);
	_rotation += amount;
	//SDL_Log("X:%f, Y:%f, Z:%f", _rotation.x, _rotation.y, _rotation.z);
};
void Transform::rotateX(GLfloat angle)
{
	adjustDirections(Vector3(angle, 0, 0));
	_rotation.x += angle;
};
void Transform::rotateY(GLfloat angle)
{
	adjustDirections(Vector3(0, angle, 0));
	_rotation.y += angle;
};
void Transform::rotateZ(GLfloat angle)
{
	adjustDirections(Vector3(0, 0, angle));
	_rotation.z += angle;
};

/**
*  <summary>
*  Move the game object in a direciton. The translation should be modified by the delta time.
*  </summary>
*/
void Transform::translate(Vector3 translation)
{
	_position += translation;
};

void Transform::translateForward(GLfloat num)
{
	_position += Vector3(_forward) * num;
};

void Transform::translateRight(GLfloat num) {
	_position += Vector3(_right) * num;
}
/**
*  <summary>
*  Move the game object in a direciton. Each axis should be modified by the delta time.
*  </summary>
*/
void Transform::translate(GLfloat x, GLfloat y, GLfloat z)
{
	_position += Vector3(x, y, z);
};