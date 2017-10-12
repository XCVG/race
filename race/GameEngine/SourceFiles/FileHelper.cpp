#include "FileHelper.h"

std::string FileHelper::loadFileFromString(std::string path)
{
	SDL_RWops *rwop_p = SDL_RWFromFile(path.c_str(), "r");

	if (rwop_p == NULL)
	{
		std::string msg = "Could not find file at ";
		msg += path;
		throw FileNotFoundException(msg);
	}

	int64_t bufferSize = SDL_RWsize(rwop_p);

	if (bufferSize <= 0)
	{
		free(rwop_p);
		return std::string();
	}

	char* buffer_p = (char*)calloc(bufferSize+1, sizeof(char));
	SDL_RWread(rwop_p, buffer_p, sizeof(char), bufferSize);

	std::string str(buffer_p);

	SDL_RWclose(rwop_p);
	free(buffer_p);

	return str;
}

std::vector<uint8_t> FileHelper::loadBinaryFileFromString(std::string path)
{
	SDL_RWops *rwop_p = SDL_RWFromFile(path.c_str(), "rb");

	if (rwop_p == NULL)
	{
		std::string msg = "Could not find file at ";
		msg += path;
		throw FileNotFoundException(msg);
	}

	int64_t bufferSize = SDL_RWsize(rwop_p);

	if (bufferSize <= 0)
	{
		free(rwop_p);
		return std::vector<uint8_t>();
	}
	
	std::vector<uint8_t> data;
	for (int64_t i = 0; i < bufferSize; i++)
	{
		uint8_t buf;
		SDL_RWread(rwop_p, &buf, sizeof(uint8_t), 1);
		data.push_back(buf);
	}

	SDL_RWclose(rwop_p);

	return data;
}

std::string FileHelper::loadFileFromStringRelative(std::string relativePath)
{
	std::string basePath = std::string(SDL_GetBasePath());
	std::string fullPath = basePath + relativePath;

	return loadFileFromString(fullPath);
}

std::vector<uint8_t> FileHelper::loadBinaryFileFromStringRelative(std::string relativePath)
{
	std::string basePath = std::string(SDL_GetBasePath());
	std::string fullPath = basePath + relativePath;

	return loadBinaryFileFromString(fullPath);
}

