#include "GameObject.h"
GameObject::GameObject()
{
    this->_position_p = new Vector3(0, 0, 0);
    this->_rotation_p = new Vector3(0, 0, 0);
    this->_scale = 1;
    this->_components_p = new std::vector<Component *>();
};
GameObject::GameObject(Vector3 _position, Vector3 _rotation, GLfloat _scale)
{

};
void GameObject::setPosition(Vector3 _position)
{
    this->_position_p = &_position;
};
void GameObject::setRotation(Vector3 _rotation)
{
    this->_rotation_p = &_rotation;
};
void GameObject::setScale(GLfloat _scale)
{
    this->_scale = _scale;
};
// void GameObject::addComponent(Component _component)
// {
//     this->_components_p->push_back(&_component); // Double checkt this
// };
void GameObject::addComponent(Component *_component_p)
{
    this->_components_p->push_back(_component_p);
};
void GameObject::removeComponent(Component _component)
{
    /**
     * TODO: This needs to be fixed
     * I need to figure out a better way to pop a component from the list
     * other than just by name (i.e., may have duplicate components (sound)).
     */
};
Vector3 GameObject::getPosition()
{
    return *this->_position_p;
};
Vector3 GameObject::getRotation()
{
    return *this->_rotation_p;
};
GLfloat GameObject::getScale()
{
    return this->_scale;
};
std::vector<Component *> GameObject::getComponentList()
{
    return *this->_components_p;
};