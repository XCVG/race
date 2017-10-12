#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#endif
#include <thread>

//temporary
static const float CUBE_ROTATE_STEP_CONST = 0.1f;
static const int RENDER_WIDTH_CONST = 1280;
static const int RENDER_HEIGHT_CONST = 720;

class RenderEngine {
public:
    std::thread* start();
    void update();
    RenderEngine();
    ~RenderEngine();
private:
    void loop();

	void setupGLOnThread();
	void setupTestSceneOnThread();
	

	void doRender();
	void setupProgram();
	void setupCube();
	void setupBaseMatrices();
	void setupFramebuffers();
	void setupFramebufferDraw();
	void drawCube();
	void drawFramebuffer();
	void updateCube();

	bool _isRunning;

	SDL_Window *_window_p;
	SDL_GLContext _context_p;

	GLuint _programID;

	GLuint _framebufferID;
	GLuint _framebufferTextureID;
	GLuint _renderbufferDepthID;

	GLuint _framebufferDrawProgramID;
	GLuint _framebufferDrawVertexArrayID;
	GLuint _framebufferDrawVertexBufferID;
	GLuint _framebufferDrawTexID;

	GLuint _cubeVertexArrayID;
	GLuint _cubeVertexBufferID;
	glm::mat4 _cubeModelViewMatrix;
	float _cubeAngle;

	GLuint _shaderMVPMatrixID;
	glm::mat4 _baseModelViewMatrix;
	glm::mat4 _baseModelViewProjectionMatrix;
};