#pragma once
#include <cstdint>

class GlobalPrefs
{
public:

	//Window vars
	static int32_t windowWidth;
	static int32_t windowHeight;
	static bool windowFullscreen;

	//Renderer vars
	static int32_t renderWidth;
	static int32_t renderHeight;
};

//Window vars
bool GlobalPrefs::windowFullscreen = false;
int32_t GlobalPrefs::windowWidth = 1280;
int32_t GlobalPrefs::windowHeight = 720;

//Renderer vars
int32_t GlobalPrefs::renderWidth = -1; //-1 means "use window size"
int32_t GlobalPrefs::renderHeight = -1;

