#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include "Vector3.h"
class Component
{
public:
    Component();
    Component(const Component &obj);
    //TODO: toString() method for each class? At least for name possibly
};