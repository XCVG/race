#pragma once
#include <glew.h>
#include <glm.hpp>
#include <vector>

enum class RendererState
{
	idle, loading, rendering, unloading
};

struct ModelData
{
	//std::string name; //don't need this, will be stored in hashtable
	GLuint vboID;
	GLuint numVerts;
	GLfloat preScale;
};

struct TextureData
{
	//std::string name; //don't need this, will be stored in hashtable
	GLuint texID; 
};

struct ModelLoadingData
{
	//TODO data to be stored while waiting for it to load
};

struct TextureLoadingData
{
	//TODO data to be stored while waiting for it to load
};