#pragma once

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLM/glm.hpp>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <glm.hpp>
#endif
#include <vector>
#include <map>

/// <summary>
/// Types of lights that can be rendered
/// </summary>
enum class RenderableLightType : int8_t
{
	AMBIENT, DIRECTIONAL, POINT, SPOT
};

/// <summary>
/// Data sent to RenderEngine on scene load
/// </summary>
struct RenderableSetupData
{
	std::map<std::string, GLuint> *models_p;
	std::map<std::string, GLuint> *textures_p;
};

/// <summary>
/// Data sent to RenderEngine to draw
/// </summary>
struct RenderableScene
{
	std::vector<RenderableObject> objects;
	std::vector<RenderableLight> lights;
	RenderableCamera camera;
};

/// <summary>
/// Camera data sent to RenderEngine on draw
/// </summary>
struct RenderableCamera
{
	glm::vec3 position;
	glm::vec3 rotation;

	glm::vec3 clearColor;
	GLfloat viewAngle;
	GLfloat nearPlane;
	GLfloat farPlane;
};

/// <summary>
/// Single object data sent to RenderEngine on draw
/// </summary>
struct RenderableObject
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	GLuint modelID;

	GLuint albedoID;
	GLuint normalID;
	GLfloat smoothness;
};

/// <summary>
/// Single light data sent to RenderEngine on draw
/// </summary>
struct RenderableLight
{
	RenderableLightType type;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::vec3 color;
	GLfloat range;
	GLfloat intensity;
	GLfloat angle;
};