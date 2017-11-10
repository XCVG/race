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
#define TOL 0.000001
#define PI 3.14159265
#include "Vector3.h"
class Quaternion
{
public:
	Quaternion();
	Quaternion(float n, float x, float y, float z);
	~Quaternion();
	GLfloat Magnitude();
	Vector3* getVector();
	GLfloat getScalar();
	Quaternion operator+=(Quaternion q);
	Quaternion operator-=(Quaternion q);
	Quaternion operator*=(GLfloat s);
	Quaternion operator/=(GLfloat s);
	Quaternion operator~(void) { return Quaternion(_n, -_v->x, -_v->y, -_v->z); };
	Quaternion operator+(Quaternion q1);

	Quaternion operator-(Quaternion q1);
	Quaternion operator*(Quaternion q);
	Quaternion operator*(GLfloat s);
	Quaternion operator*(Vector3 v);
	Quaternion operator/(GLfloat s);

	GLfloat QGetAngle();

	Vector3 QGetAxis();

	Quaternion QRotate(Quaternion q1, Quaternion q2);

	Quaternion MakeQFromEulerAngles(float x, float y, float z);

	Vector3 MakeEulerAnglesFromQ(Quaternion q);

private:
	GLfloat _n;
	Vector3 *_v;
};

inline Quaternion::Quaternion()
{
	_n = 0;
	_v = new Vector3(0, 0, 0);
};

inline Quaternion::Quaternion(float n, float x, float y, float z)
{
	_n = n;
	_v = new Vector3(x, y, z);
};

inline Quaternion::~Quaternion()
{
	delete(_v);
};

inline GLfloat Quaternion::Magnitude()
{
	return (GLfloat)sqrtf((_n*_n) + (_v->x*_v->x) + (_v->y*_v->y) + (_v->z*_v->z));
};

inline Vector3* Quaternion::getVector() 
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
	_v->x += q._v->x;
	_v->y += q._v->y;
	_v->z += q._v->z;
	return *this;
};

inline Quaternion Quaternion::operator-=(Quaternion q)
{
	_n -= q._n;
	_v->x -= q._v->x;
	_v->y -= q._v->y;
	_v->z -= q._v->z;
	return *this;
};

inline Quaternion Quaternion::operator*=(GLfloat s)
{
	_n *= s;
	_v->x *= s;
	_v->y *= s;
	_v->z *= s;
	return *this;
};

inline Quaternion Quaternion::operator/=(GLfloat s)
{
	_n /= s;
	_v->x /= s;
	_v->y /= s;
	_v->z /= s;
	return *this;
};

inline Quaternion Quaternion::operator+(Quaternion q1)
{
	return Quaternion((this->_n + q1._n), (this->_v->x + q1._v->x), (this->_v->y + q1._v->y), (this->_v->z + q1._v->z));
};

inline Quaternion Quaternion::operator-(Quaternion q1)
{
	return Quaternion((this->_n - q1._n), (this->_v->x - q1._v->x), (this->_v->y - q1._v->y), (this->_v->z - q1._v->z));
};

inline Quaternion Quaternion::operator*(Quaternion q) 
{
	return Quaternion((this->_n * this->_n - this->_v->x * this->_v->x - this->_v->y * this->_v->y - this->_v->z * this->_v->z),
		(this->_n * q._v->x + this->_v->x * q._n + this->_v->y * q._v->z - this->_v->z * q._v->y),
		(this->_n * q._v->y + this->_v->y * q._n + this->_v->z * q._v->x - this->_v->x * q._v->z),
		(this->_n * q._v->z + this->_v->z * q._n + this->_v->x * q._v->y - this->_v->y * q._v->x));
};

inline Quaternion Quaternion::operator*(GLfloat s) 
{
	return Quaternion(this->_n * s, this->_v->x * s, this->_v->y * s, this->_v->z * s);
};

inline Quaternion Quaternion::operator*(Vector3 v)
{
	return Quaternion(-(this->_v->x * v.x + this->_v->y * v.y + this->_v->z * v.z),
		(this->_n * v.x + this->_v->z * v.y - this->_v->y * v.z),
		(this->_n * v.y + this->_v->x * v.z - this->_v->z * v.x),
		(this->_n * v.z + this->_v->y * v.x - this->_v->x * v.y));
};

inline Quaternion Quaternion::operator/(GLfloat s)
{
	return Quaternion(this->_n / s, this->_v->x / s, this->_v->y / s, this->_v->z / s);
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

inline Vector3* QVRotate(Quaternion q, Vector3 v) 
{
	Quaternion t;
	t = q * v * (~q);
	return t.getVector();
};

inline Quaternion Quaternion::MakeQFromEulerAngles(float x, float y, float z)
{
	Quaternion q;
	double roll = x;
	double pitch = y;
	double yaw = z;

	double cyaw, cpitch, croll, syaw, spitch, sroll;
	double cyawcpitch, syawspitch, cyawspitch, syawcpitch;

	cyaw = cosf(0.5f * yaw);
	cpitch = cosf(0.5f * pitch);
	croll = cosf(0.5f * roll);
	syaw = sinf(0.5f * yaw);
	spitch = sinf(0.5f * pitch);
	sroll = sinf(0.5f * roll);

	cyawcpitch = cyaw * cpitch;
	syawspitch = syaw * spitch;
	cyawspitch = cyaw * spitch;
	syawcpitch = syaw * cpitch;

	q._n = (GLfloat)(cyawcpitch * croll + syawspitch * sroll);
	q._v->x = (GLfloat)(cyawcpitch * sroll - syawspitch * croll);
	q._v->y = (GLfloat)(cyawspitch * croll + syawcpitch * sroll);
	q._v->z = (GLfloat)(syawspitch * croll - cyawspitch * sroll);
	return q;
};

inline Vector3 Quaternion::MakeEulerAnglesFromQ(Quaternion q) 
{
	double r11, r21, r31, r32, r33, r12, r13;
	double q00, q11, q22, q33;
	double temp;
	Vector3 u;
	q00 = q._n * q._n;
	q11 = q._v->x * q._v->x;
	q22 = q._v->y * q._v->y;
	q33 = q._v->z * q._v->z;
	
	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (q._v->x * q._v->y + q._n * q._v->z);
	r31 = 2 * (q._v->x * q._v->z - q._n * q._v->y);
	r32 = 2 * (q._v->y * q._v->z + q._n * q._v->x);

	temp = fabs(r31);
	if (temp > 0.9999999) {
		r12 = 2 * (q._v->x * q._v->y - q._n * q._v->z);
		r13 = 2 * (q._v->x * q._v->z + q._n * q._v->y);

		u.x = 0.0f;
		u.y = (-(PI / 2) * r31 / temp);
		u.z = (atan2(-r12, -r31 * r13));
		return u;
	}
	u.x = atan2(r32, r33);
	u.y = asin(-r31);
	u.z = atan2(r21, r11);
	return u;
};