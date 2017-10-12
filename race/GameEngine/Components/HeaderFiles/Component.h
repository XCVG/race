#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/vec3.hpp>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_opengl.h>
#include <vec3.hpp>
#endif
#include <string>
class Component
{
public:
    Component();
    Component(const Component &obj);
    virtual std::string ToString();
};