#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include "Component.h"
#include "Vector3.h"
class ColliderComponent : public Component
{
public:
	ColliderComponent();
	ColliderComponent(Vector3 _vertices);
	ColliderComponent(const ColliderComponent& obj);
	void setVertices(Vector3 _vertices);
	Vector3 getVertices();
private:
	Vector3 _vertices; // TODO: This needs to be different
};