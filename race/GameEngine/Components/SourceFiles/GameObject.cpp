#include "GameObject.h"
void setPosition(Vector3 _position)
{
    this->_position = _position;
};
void setRotation(Vector3 _rotation)
{
    this->_rotation = _rotation;
};
void setScale(GLfloat _scale)
{
    this->_scale = _scale;
};
void addComponent(Component _component)
{
    this->_components.add(_component); // Double checkt this
};
void removeComponent(Component _component)
{
    /**
     * TODO: This needs to be fixed
     * I need to figure out a better way to pop a component from the list
     * other than just by name (i.e., may have duplicate components (sound)).
     */
};
Vector3 getPosition()
{
    return this->_position;
};
Vector3 getRotation()
{
    return this->_rotation;
};
GLfloat getScale()
{
    return this->_scale;
};
std::list<Component> getComponentList()
{
    return this->_components;
};