#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include <string>
#include "Component.h"
class RenderComponent : public Component
{
public:
	void setModelName(std::string _name);
	void setAlbedoName(std::string _name);
	void setNormalName(std::string _name);
	void setSmoothness(GLfloat _value);
	std::string getModelName();
	std::string getAlbedoName();
	std::string getNormalName();
	GLfloat getSmoothness();
private:
	std::string _modelName;
	std::string _albedoName;
	std::string _normalName;
	GLfloat _smoothness;
};