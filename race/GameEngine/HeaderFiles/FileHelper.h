#pragma once
#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <string>
#include <exception>
#include <SDL.h>
#include <SDL_filesystem.h>

/// <summary>
/// Static helper class that wraps some SDL file operations
/// </summary>
class FileHelper
{
public:

	/// <summary>
	/// Loads the contents of a file into a string
	/// </summary>
	/// <param name="path">String containing an absolute path to the file</param>
	/// <return>Contents of the file specified</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::string loadFileFromString(std::string path);

	/// <summary>
	/// Loads the contents of a file into a string (relative path)
	/// </summary>
	/// <param name="relativePath">String containing a path to the file relative to the application executable</param>
	/// <return>Contents of the file specified</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::string loadFileFromStringRelative(std::string relativePath);
};

/// <summary>
/// Exception thrown by FileHelper when it can't find a file
/// </summary>
class FileNotFoundException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Invalid file!";
	}
};

#endif