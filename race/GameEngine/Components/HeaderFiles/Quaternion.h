#pragma once
#ifdef __APPLE__
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <SDL2/SDL.h> 
#include <SDL2/SDL_opengl.h> 
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <SDL.h> 
#endif
#define PI 3.14159265
#include "Vector3.h"
class Quaternion
{
public:
	Quaternion();
	Quaternion(float n, float x, float y, float z);
	~Quaternion();
	GLfloat Magnitude();
	Vector3 getVector();
	GLfloat getScalar();
	Quaternion operator+=(Quaternion q);
	Quaternion operator-=(Quaternion q);
	Quaternion operator*=(GLfloat s);
	Quaternion operator/=(GLfloat s);
	Quaternion operator~(void) { return Quaternion(_n, -_v.x, -_v.y, -_v.z); };
	Quaternion operator+(Quaternion q1);

	Quaternion operator-(Quaternion q1);
	Quaternion operator*(Quaternion q);
	Quaternion operator*(GLfloat s);
	Quaternion operator*(Vector3 v);
	Quaternion operator/(GLfloat s);

	GLfloat QGetAngle();
	Vector3 QGetAxis();
	Quaternion QRotate(Quaternion q1, Quaternion q2);
	Quaternion& MakeQFromEulerAngles(float x, float y, float z);
	Vector3 MakeEulerAnglesFromQ();

	Quaternion& CreateFromAxisAngle(Vector3 vec, GLfloat angle);

private:
	GLfloat _n;
	Vector3 _v;
};

inline Quaternion::Quaternion()
{
	_n = 0;
	_v = Vector3(0, 0, 0);
};

inline Quaternion::Quaternion(float n, float x, float y, float z)
{
	_n = n;
	_v = Vector3(x, y, z);
};

inline Quaternion::~Quaternion()
{
};

inline GLfloat Quaternion::Magnitude()
{
	return (GLfloat)sqrtf((_n*_n) + (_v.x*_v.x) + (_v.y*_v.y) + (_v.z*_v.z));
};

inline Vector3 Quaternion::getVector() 
{
	return _v;
};

inline GLfloat Quaternion::getScalar() 
{
	return _n;
};

inline Quaternion Quaternion::operator+=(Quaternion q)
{
	_n += q._n;
	_v.x += q._v.x;
	_v.y += q._v.y;
	_v.z += q._v.z;
	return *this;
};

inline Quaternion Quaternion::operator-=(Quaternion q)
{
	_n -= q._n;
	_v.x -= q._v.x;
	_v.y -= q._v.y;
	_v.z -= q._v.z;
	return *this;
};

inline Quaternion Quaternion::operator*=(GLfloat s)
{
	_n *= s;
	_v.x *= s;
	_v.y *= s;
	_v.z *= s;
	return *this;
};

inline Quaternion Quaternion::operator/=(GLfloat s)
{
	_n /= s;
	_v.x /= s;
	_v.y /= s;
	_v.z /= s;
	return *this;
};

inline Quaternion Quaternion::operator+(Quaternion q1)
{
	return Quaternion((this->_n + q1._n), (this->_v.x + q1._v.x), (this->_v.y + q1._v.y), (this->_v.z + q1._v.z));
};

inline Quaternion Quaternion::operator-(Quaternion q1)
{
	return Quaternion((this->_n - q1._n), (this->_v.x - q1._v.x), (this->_v.y - q1._v.y), (this->_v.z - q1._v.z));
};

inline Quaternion Quaternion::operator*(Quaternion q)
{
	return Quaternion((this->_n * q._n - this->_v.x * q._v.x - this->_v.y * q._v.y - this->_v.z * q._v.z),
		(this->_n * q._v.x + this->_v.x * q._n + this->_v.y * q._v.z - this->_v.z * q._v.y),
		(this->_n * q._v.y + this->_v.y * q._n + this->_v.z * q._v.x - this->_v.x * q._v.z),
		(this->_n * q._v.z + this->_v.z * q._n + this->_v.x * q._v.y - this->_v.y * q._v.x));
};

inline Quaternion Quaternion::operator*(GLfloat s) 
{
	return Quaternion(this->_n * s, this->_v.x * s, this->_v.y * s, this->_v.z * s);
};

inline Quaternion Quaternion::operator*(Vector3 v)
{
	return Quaternion(-(this->_v.x * v.x + this->_v.y * v.y + this->_v.z * v.z),
		(this->_n * v.x + this->_v.z * v.y - this->_v.y * v.z),
		(this->_n * v.y + this->_v.x * v.z - this->_v.z * v.x),
		(this->_n * v.z + this->_v.y * v.x - this->_v.x * v.y));
};

inline Quaternion Quaternion::operator/(GLfloat s)
{
	return Quaternion(this->_n / s, this->_v.x / s, this->_v.y / s, this->_v.z / s);
};

inline GLfloat Quaternion::QGetAngle() {
	return (GLfloat)(2 * acosf(this->_n));
};

inline Vector3 Quaternion::QGetAxis() {
	Vector3 v;
	float m;
	v = this->getVector();
	m = v.magnitude();

	if (m <= TOL) {
		return Vector3();
	}
	else {
		return v / m;
	}
};

inline Quaternion Quaternion::QRotate(Quaternion q1, Quaternion q2) 
{
	return q1 * q2 * (~q1);
};

inline Vector3 QVRotate(Quaternion q, Vector3 v) 
{
	Quaternion t;
	t = q * v * (~q);
	return t.getVector();
};

inline Quaternion& Quaternion::MakeQFromEulerAngles(float x, float y, float z)
{
	double roll = x;
	double pitch = y;
	double yaw = z;

	double cyaw, cpitch, croll, syaw, spitch, sroll;
	double cyawcpitch, syawspitch, cyawspitch, syawcpitch;

	cyaw = cos(0.5f * yaw);
	cpitch = cos(0.5f * pitch);
	croll = cos(0.5f * roll);
	syaw = sin(0.5f * yaw);
	spitch = sin(0.5f * pitch);
	sroll = sin(0.5f * roll);

	cyawcpitch = cyaw * cpitch;
	syawspitch = syaw * spitch;
	cyawspitch = cyaw * spitch;
	syawcpitch = syaw * cpitch;

	this->_n = (GLfloat)(cyawcpitch * croll + syawspitch * sroll);
	this->_v.x = (GLfloat)(cyawcpitch * sroll - syawspitch * croll);
	this->_v.y = (GLfloat)(cyawspitch * croll + syawcpitch * sroll);
	this->_v.z = (GLfloat)(syawspitch * croll - cyawspitch * sroll);
	return *this;
};

inline Vector3 Quaternion::MakeEulerAnglesFromQ() 
{
	Vector3 u;

	GLfloat sinr = 2.0f * (this->_n * this->_v.x + this->_v.y * this->_v.z);
	GLfloat cosr = 1.0f - (2.0f * (this->_v.x * this->_v.x + this->_v.y * this->_v.y));
	u.x = (GLfloat)atan2(sinr, cosr);

	GLfloat sinp = 2.0f * (this->_n * this->_v.y - this->_v.z * this->_v.x);
	if (fabs(sinp) >= 1)
		u.y = (GLfloat)copysign(PI / 2, sinp);
	else
		u.y = (GLfloat)asin(sinp);

	GLfloat siny = 2.0f * (this->_n * this->_v.z + this->_v.x * this->_v.y);
	GLfloat cosy = 1.0f - (2.0f * (this->_v.y * this->_v.y + this->_v.z * this->_v.z));
	u.z = (GLfloat)atan2(siny, cosy);
	return u;
};

inline Quaternion& Quaternion::CreateFromAxisAngle(Vector3 vec, GLfloat angle)
{
	GLfloat s = sinf(angle / 2.0f);
	this->_v.x = vec.x * s;
	this->_v.y = vec.y * s;
	this->_v.z = vec.z * s;
	this->_n = cosf(angle / 2.0f);
	return *this;
}