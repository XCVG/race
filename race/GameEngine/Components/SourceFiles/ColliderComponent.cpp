#include "ColliderComponent.h"
ColliderComponent::ColliderComponent()
{
    this->_vertices = new glm::vec3(); // TODO: Fix this
};
ColliderComponent::ColliderComponent(glm::vec3 _vertices)
{
    this->_vertices = _vertices;
};
ColliderComponent::ColliderComponent(glm::vec3 *_vertices)
{
    this->_vertices = *_vertices;
};
ColliderComponent::ColliderComponent(const ColliderComponent& obj)
{
    this->_vertices = obj._vertices;
};
void ColliderComponent::setVertices(glm::vec3 _vertices)
{
    this->_vertices = _vertices;
};
glm::vec3 ColliderComponent::getVertices()
{
    return this->_vertices;
};