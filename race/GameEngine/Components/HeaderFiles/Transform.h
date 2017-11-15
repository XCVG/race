#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif
#include "Vector3.h"
#include "Quaternion.h"
class Transform
{
public:
    Transform();
    Transform(Vector3 _position, Vector3 _rotation, GLfloat _scale);
    Transform(Vector3 *_position, Vector3 *_rotation, GLfloat _scale);
    Transform(const Transform &obj);
	bool operator==(const Transform& t);
#pragma region Rotation Calculation Methods
	Vector3 rotateAround(Vector3 distance, Vector3 objectPos, Vector3 rotation);
	void rotate(Vector3 amount);
	void rotateX(GLfloat angle);
	void rotateY(GLfloat angle);
	void rotateZ(GLfloat angle);
#pragma endregion
#pragma region Translation Calculation Methods
	void translate(Vector3 translation);
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void translateForward(GLfloat);
	void translateRight(GLfloat num);
#pragma endregion
    void setPosition(Vector3 _position);
	void setRotation(Vector3 _rotation);
    void setScale(GLfloat _scale);
    Vector3 getPosition();
	Vector3 getRotation();
	GLfloat getScale();
	Vector3 getForward();
	void adjustDirections(Vector3);
	////////////////////////////////
    Vector3 _position;
    Vector3 _rotation;
    GLfloat _scale;
	Vector3 _forward;
	Vector3 _right;
	Vector3 _up;
	Quaternion _orientation;
private:
};
inline bool Transform::operator==(const Transform& t)
{
	return (this->_position == t._position &&
			this->_rotation == t._rotation &&
			this->_scale == t._scale);
};