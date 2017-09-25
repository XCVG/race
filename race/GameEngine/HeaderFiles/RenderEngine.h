#pragma once
#include <thread>
#include <queue>
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <SDL.h>
#include <RenderableTypes.h>
#include <RendererInternalTypes.h> //which will end up included in everything that includes this... sigh

//temporary
static const float CUBE_ROTATE_STEP_CONST = 0.1f;
static const int RENDER_WIDTH_CONST = 1280;
static const int RENDER_HEIGHT_CONST = 720;

//probably ought to move to PIMPL model for various reasons, will try next week
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

	//threaded render loop
    void loop();

	
	//setup methods
	void setupGLOnThread();
	void setupTestSceneOnThread();
	
	//render methods (some temporary)
	void doRender();
	void setupProgram();
	void setupCube();
	void setupBaseMatrices();
	void setupFramebuffers();
	void setupFramebufferDraw();
	void drawCube();
	void drawFramebuffer();
	void updateCube();

	//context and window
	SDL_Window *_window_p;
	SDL_GLContext _context_p;

	//TODO message queue
	std::queue<RenderMessageData> *_mq_p;

	//resource lists
	std::vector<ModelData> *_models_p;
	std::vector<TextureData> *_textures_p;

	//threading stuff
	bool _isRunning;
	std::thread *_renderThread_p;

	//honestly not sure
	GLuint _programID;

	//framebuffer stuff
	GLuint _framebufferID;
	GLuint _framebufferTextureID;
	GLuint _renderbufferDepthID;

	GLuint _framebufferDrawProgramID;
	GLuint _framebufferDrawVertexArrayID;
	GLuint _framebufferDrawVertexBufferID;
	GLuint _framebufferDrawTexID;

	//temporary cube stuff
	GLuint _cubeVertexArrayID;
	GLuint _cubeVertexBufferID;
	glm::mat4 _cubeModelViewMatrix;
	float _cubeAngle;

	//base MVP, may keep or remove
	GLuint _shaderMVPMatrixID;
	glm::mat4 _baseModelViewMatrix;
	glm::mat4 _baseModelViewProjectionMatrix;
};