#pragma once
#ifndef OBJIMPORT_H
#define OBJIMPORT_H
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <glew.h>
#include <SDL.h>
#include <SDL_filesystem.h>

class OBJImport {
public:

	/// <summary>
	/// Loads info from an .obj file to vector GLfloat array to use for render engine
	///
	/// </summary>
	static std::vector<GLfloat> importObjInfo(std::string string);

};

#endif