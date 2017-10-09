#pragma once
#if defined __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_filesystem.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_filesystem.h>
#endif
#include <string>
#include <exception>
#include <vector>
#include <memory>

/// <summary>
/// Static helper class that wraps some SDL file operations
/// </summary>
class FileHelper {
public:

	/// <summary>
	/// Loads the contents of a file into a string
	/// </summary>
	/// <param name="path">String containing an absolute path to the file</param>
	/// <return>Contents of the file specified</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::string loadFileFromString(std::string path);

	/// <summary>
	/// Loads the contents of a file into a binary array
	/// </summary>
	/// <param name="path">String containing an absolute path to the file</param>
	/// <return>Contents of the file specified (binary)</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::vector<uint8_t> loadBinaryFileFromString(std::string path);

	/// <summary>
	/// Loads the contents of a file into a string (relative path)
	/// </summary>
	/// <param name="relativePath">String containing a path to the file relative to the application executable</param>
	/// <return>Contents of the file specified</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::string loadFileFromStringRelative(std::string relativePath);

	/// <summary>
	/// Loads the contents of a file into a binary array
	/// </summary>
	/// <param name="relativePath">String containing a path to the file relative to the application executable</param>
	/// <return>Contents of the file specified (binary)</return>
	/// <throws>FileNotFoundException if file is not found</throws>
	static std::vector<uint8_t> loadBinaryFileFromStringRelative(std::string relativePath);
};

/// <summary>
/// Exception thrown by FileHelper when it can't find a file
/// </summary>
class FileNotFoundException : public std::runtime_error {
public:
	FileNotFoundException(const std::string& what_arg) : std::runtime_error(what_arg) {}
};