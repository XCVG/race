#include "RenderEngine.h"
#include "Shaders.h"
#include "Quad.h"
#include "Cube.h"
#include "main.h"

RenderEngine::RenderEngine()
{
	//constructor: on engine thread
}

void RenderEngine::start()
{
	//start: on engine thread

	_window_p = g_window_p;

	_cubeAngle = 0;

	//TODO subscribe to messaging

	//spawn thread
	_isRunning = true;
	std::thread rthread(&RenderEngine::loop, this);
	_renderThread_p = &rthread;
}

void RenderEngine::update()
{
	//update: on engine thread, synchronized with game loop
}

RenderEngine::~RenderEngine()
{
    //destructor

	_isRunning = false;
	_renderThread_p->join();
	delete(_renderThread_p);
}

void RenderEngine::loop()
{
	//initial setup: run once

	SDL_Log("RenderEngine thread started!");

	setupGLOnThread();
	setupTestSceneOnThread();

	//loop: on RenderEngine thread
	while (_isRunning)
	{
		doRender(); //this should run really absurdly fast
	}

	SDL_Log("RenderEngine thread halted!");

	//cleanup after run
}

void RenderEngine::setupGLOnThread()
{
	//this should work in theory as long as we don't need a GL context on the main thread
	//which would slow things down
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	_context_p = SDL_GL_CreateContext(_window_p);
	glewExperimental = GL_TRUE;
	glewInit();
}

void RenderEngine::setupTestSceneOnThread()
{
	setupProgram();
	setupFramebuffers();
	setupFramebufferDraw();
	setupBaseMatrices();
	setupCube();
}

/// <summary>
/// Renders a frame
/// </summary>
void RenderEngine::doRender()
{
	updateCube();

	//cycleBackground();
	//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawCube();
	drawFramebuffer();

	SDL_GL_SwapWindow(_window_p);
}

void RenderEngine::setupProgram()
{
	_programID = LoadShaders();
}

void RenderEngine::setupCube()
{
	glGenVertexArrays(1, &_cubeVertexArrayID);
	glBindVertexArray(_cubeVertexArrayID);

	glGenBuffers(1, &_cubeVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexBufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderEngine::setupBaseMatrices()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)16 / (float)9, 0.1f, 100.0f); //TODO global const screen size instead of hardcoding it stupid
	glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0f);
	_baseModelViewMatrix = view * model;
	_baseModelViewProjectionMatrix = projection * view * model;

	_shaderMVPMatrixID = glGetUniformLocation(_programID, "MVP");
}

void RenderEngine::setupFramebuffers()
{
	//gen FBO
	glGenFramebuffers(1, &_framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

	//gen framebuffer texture
	glGenTextures(1, &_framebufferTextureID);
	glBindTexture(GL_TEXTURE_2D, _framebufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RENDER_WIDTH_CONST, RENDER_HEIGHT_CONST, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//gen depthbuffer
	glGenRenderbuffers(1, &_renderbufferDepthID);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferDepthID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RENDER_WIDTH_CONST, RENDER_HEIGHT_CONST);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderbufferDepthID);

	//configure FBO
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _framebufferTextureID, 0);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	//quick check
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SDL_Log("Failed to setup framebuffer!");
		throw;
	}

	//unbind all
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_framebufferDrawTexID = glGetUniformLocation(_framebufferDrawProgramID, "texture");

}

void RenderEngine::setupFramebufferDraw()
{
	glGenVertexArrays(1, &_framebufferDrawVertexArrayID);
	glBindVertexArray(_framebufferDrawVertexArrayID);

	glGenBuffers(1, &_framebufferDrawVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _framebufferDrawVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_framebufferDrawProgramID = LoadShadersFBDraw();

}

void RenderEngine::drawCube()
{

	//set shader program
	glUseProgram(_programID);

	//bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
	glViewport(0, 0, RENDER_WIDTH_CONST, RENDER_HEIGHT_CONST);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//bind cube, set properties, and draw
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glBindVertexArray(_cubeVertexArrayID);
	glm::mat4 cubeMVPM = _baseModelViewProjectionMatrix *  _cubeModelViewMatrix;
	glUniformMatrix4fv(_shaderMVPMatrixID, 1, GL_FALSE, &cubeMVPM[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::drawFramebuffer()
{
	//setup framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int w, h;
	SDL_GL_GetDrawableSize(_window_p, &w, &h);
	glViewport(0, 0, w, h);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//setup shader
	glUseProgram(_framebufferDrawProgramID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _framebufferTextureID);
	glUniform1i(_framebufferDrawTexID, 0);

	//setup vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _framebufferDrawVertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	//draw
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);

}

void RenderEngine::updateCube()
{
	_cubeAngle += CUBE_ROTATE_STEP_CONST;
	glm::mat4 identity = glm::mat4();
	glm::mat4 rotate = glm::rotate(identity, _cubeAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	_cubeModelViewMatrix = rotate;
}