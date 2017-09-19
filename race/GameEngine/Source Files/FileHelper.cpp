#include "../HeaderFiles/FileHelper.h"

std::string FileHelper::loadFileFromString(std::string path)
{
	std::string basePath = std::string(SDL_GetBasePath());
	std::string fullPath = basePath + path;

	SDL_RWops *rwop_p = SDL_RWFromFile(fullPath.c_str(), "r");

	if (rwop_p == NULL)
	{
		//SDL_Log(SDL_GetError());
		throw FileNotFoundException();
	}

	int64_t bufferSize = SDL_RWsize(rwop_p);

	if (bufferSize < 0)
	{
		free(rwop_p);
		return std::string();
	}

	char* buffer_p = (char*)malloc(bufferSize);
	SDL_RWread(rwop_p, buffer_p, sizeof(char), bufferSize);
	
	std::string str(buffer_p);

	SDL_RWclose(rwop_p);
	free(buffer_p);

	return str;
}

