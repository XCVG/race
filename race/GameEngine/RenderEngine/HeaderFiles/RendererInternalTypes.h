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