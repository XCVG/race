#pragma once

//temporary consts, will replace with config system
static const float CUBE_ROTATE_STEP_CONST = 0.1f;
static const int RENDER_WIDTH_CONST = 1280;
static const int RENDER_HEIGHT_CONST = 720;

//forward declaration of class makes PIMPL possible
class RenderEngineImplementation;

class RenderEngine {
public:
	/// <summary>
	/// Starts the RenderEngine, creates the rendering thread, and switches OpenGL context
	/// </summary>
    void start();

	/// <summary>
	/// Run by engine on every tick, synchronized update on engine thread
	/// </summary>
    void update();

    RenderEngine();
    ~RenderEngine();
private:
	RenderEngineImplementation *_impl;
	
};