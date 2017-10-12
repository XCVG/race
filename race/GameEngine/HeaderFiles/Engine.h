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
#include <thread>
#include "EngineHeaders.h"
#include "ComponentHeaders.h"
#include "GameObject.h"
class Engine {
public:
    Engine();
    ~Engine();
    void start();
    void update();
    void stop();
    std::thread *_engineThread_p;
private:
	FileEngine *_fileEngine_p;
    RenderEngine *_renderEngine_p;
    PhysicsEngine *_physicsEngine_p;
    AIEngine *_aiEngine_p;
    InputEngine *_inputEngine_p;
    SoundEngine *_soundEngine_p;	
    std::thread *_physicsThread_p;
    std::thread *_aiThread_p;
    bool _running = false;
    void loop();
};
