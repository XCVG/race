#pragma once
#include <glew.h>
#include <glm.hpp>
#include <vector>

struct RenderMessageData
{
	//TODO make this sane
	void *data;
};

struct ModelData
{
	std::string name;
	//GLuint vboID; //don't need this because we'll use it as the index
	GLuint numVerts;
	GLfloat preScale;
};

struct TextureData
{
	std::string name;
	//GLuint texID; //don't need this because we'll use it as the index
};