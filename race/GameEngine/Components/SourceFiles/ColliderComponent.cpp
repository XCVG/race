#include "ColliderComponent.h"
void ColliderComponent::setVertices(Vector3 _verts)
{
    this->_vertices = _verts;
};
Vector3 ColliderComponent::getVertices()
{
    return this->_vertices;
};