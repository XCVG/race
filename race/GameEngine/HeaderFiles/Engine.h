#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#endif
#include <string>
#include <iostream>
#include "RenderEngine.h"
#include "PhysicsEngine.h"
#include "AIEngine.h"
#include "InputEngine.h"
#include "SoundEngine.h"

class Engine {
public:
    void start();
    void update();
    Engine();
    ~Engine();
private:
    RenderEngine* _renderEngine;
    PhysicsEngine* _physicsEngine;
    AIEngine* _aiEngine;
    InputEngine* _inputEngine;
    SoundEngine* _soundEngine;
    void loop();
};
