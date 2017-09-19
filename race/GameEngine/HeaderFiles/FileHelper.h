#pragma once
#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>
#include <exception>
#include <SDL.h>
#include <SDL_filesystem.h>

class FileHelper
{
public:
	static std::string loadFileFromString(std::string path);

};

class FileNotFoundException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Invalid file!";
	}
};

#endif