#include "Vector3.h"
Vector3::Vector3()
{
    this->_x = 0;
    this->_y = 0;
    this->_z = 0;
};
Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
};
Vector3::Vector3(const Vector3 &obj)
{
    this->_x = obj._x;
    this->_y = obj._y;
    this->_z = obj._x;
};