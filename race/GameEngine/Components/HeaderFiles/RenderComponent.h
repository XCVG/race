#pragma once
#include "Component.h"
#include <string>
#include <SDL_opengl.h>
class RenderComponent : public Component
{
public:
	std::string _modelName;
	std::string _albedoName;
	std::string _normalName;
	GLfloat _smoothness;
};