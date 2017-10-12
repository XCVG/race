#pragma once
#include "Component.h"
class ColliderComponent : public Component
{
public:
	ColliderComponent();
	ColliderComponent(glm::vec3 _vertices);
	ColliderComponent(glm::vec3 *_vertices);
	ColliderComponent(const ColliderComponent& obj);
	void setVertices(glm::vec3 _vertices);
	glm::vec3 getVertices();
private:
	glm::vec3 _vertices; // TODO: This needs to be different
};