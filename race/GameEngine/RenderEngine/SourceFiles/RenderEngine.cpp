#include "RenderEngine.h"

#include <thread>
#include <queue>
#include <atomic>
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <SDL.h>
#include <MessageReceiver.h>
#include <MessageTypes.h>
#include <GlobalPrefs.h>

#include "RenderableTypes.h"
#include "RendererInternalTypes.h"
#include "RenderMessageReceiver.h"
#include "RenderFileMessageReceiver.h"

#include "Shaders.h"
#include "Quad.h"
#include "Cube.h"
#include "main.h"

const std::string MODEL_BASEPATH_CONST = "ResourceFiles/Models/";
const std::string TEXTURE_BASEPATH_CONST = "ResourceFiles/Textures/";
const std::string MODEL_EXTENSION_CONST = ".obj";
const std::string TEXTURE_EXTENSION_CONST = ".tga";

class RenderEngineImplementation
{
public:
	RenderEngineImplementation()
	{
		//constructor: on engine thread
	}

	void start()
	{
		//start: on engine thread

		_window_p = g_window_p; //really ought to switch to dependency injection

		_state = RendererState::idle;

		_cubeAngle = 0;

		//create message queue and handler, then subscribe to messaging
		_mq_p = new std::vector<std::shared_ptr<Message>>();
		_mr_p = new RenderMessageReceiver(_mq_p);
		_mr_p->subscribeAll();
		_mqMutex_p = new std::mutex();
		
		//create file handling message queue and handler, then subscribe to messaging
		_fmq_p = new std::vector<std::shared_ptr<Message>>();
		_fmr_p = new RenderFileMessageReceiver(_fmq_p);
		_fmr_p->subscribeAll();
		_fmqMutex_p = new std::mutex();

		//spawn thread
		_isRunning = true;
		_renderThread_p = new std::thread(&RenderEngineImplementation::loop, this);
	}

	void update()
	{
		//update: on engine thread, synchronized with game loop
	}

	~RenderEngineImplementation()
	{
		//destructor

		//kill render thread first
		_isRunning = false;
		_renderThread_p->join();
		delete(_renderThread_p);

		delete(_mr_p); //rely on destructor to take care of resource release
		delete(_mq_p); //this also deletes everything in the message queue
		delete(_mqMutex_p);

		delete(_fmr_p);
		delete(_fmq_p);
		delete(_fmqMutex_p);
	}

private:

	//TODO: some should be std::atomic

	//context and window
	SDL_Window *_window_p;
	SDL_GLContext _context_p;

	//state data
	std::atomic<RendererState> _state;
	RenderableScene *_lastScene_p;
	RenderableOverlay *_lastOverlay_p;

	//messaging stuff
	RenderMessageReceiver *_mr_p;
	RenderFileMessageReceiver *_fmr_p;
	std::vector<std::shared_ptr<Message>> *_mq_p;
	std::vector<std::shared_ptr<Message>> *_fmq_p;
	std::mutex *_mqMutex_p;
	std::mutex *_fmqMutex_p;

	//resource lists
	std::map<std::string, ModelData> *_models_p;
	std::map<std::string, TextureData> *_textures_p;
	std::vector<ModelLoadingData> *_modelLoadQueue_p;
	std::vector<TextureLoadingData> *_textureLoadQueue_p;

	//threading stuff
	bool _isRunning;	
	std::thread *_renderThread_p;

	//honestly not sure
	GLuint _programID;

	//framebuffer stuff
	int _renderWidth;
	int _renderHeight;

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

	void loop()
	{
		//initial setup: run once

		SDL_Log("RenderEngine thread started!");

		setupStructuresOnThread();
		setupGLOnThread();
		setupSceneOnThread();

		//for testing
		SDL_Log(std::to_string(SDL_GL_GetSwapInterval()).c_str());
		_state = RendererState::rendering;

		//loop: on RenderEngine thread
		while (_isRunning)
		{
			//TODO: state switching and stuff
			//doLoad, doRender/doImmediateLoad, doUnload

			checkQueue();

			switch (_state)
			{
			case RendererState::idle:
				std::this_thread::sleep_for(std::chrono::milliseconds(10)); //don't busywait!
				break;
			case RendererState::loading:
				doLoad();
				break;
			case RendererState::rendering:
				doSingleLoad();
				doRender(); //this should run really absurdly fast
				break;
			case RendererState::unloading:
				doUnload();
				break;
			}

			
			
			//std::this_thread::sleep_for(std::chrono::milliseconds(17));
		}

		//force unload/release if we're not already unloaded
		if (_state != RendererState::idle)
		{
			doUnload();
		}

		SDL_Log("RenderEngine thread halted!");

		//cleanup after run
		cleanupGLOnThread();
		cleanupStructuresOnThread();
	}

	void setupStructuresOnThread()
	{
		//setup data structures

		_models_p = new std::map<std::string, ModelData>();
		_textures_p = new std::map<std::string, TextureData>();
		_modelLoadQueue_p = new std::vector<ModelLoadingData>();
		_textureLoadQueue_p = new std::vector<TextureLoadingData>();
	}

	void setupGLOnThread()
	{
		//this should work in theory as long as we don't need a GL context on the main thread
		//which would slow things down
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
		//SDL_GL_CreateContext(_window_p);
		_context_p = SDL_GL_CreateContext(_window_p); //we will need to modify this to release/acquire context in concert with the UI thread
		SDL_GL_SetSwapInterval(1);
		//SDL_GL_MakeCurrent(g_window_p, g_context);
		glewExperimental = GL_TRUE;
		glewInit();
	}

	void setupSceneOnThread()
	{
		setupWindow();
		setupProgram();
		setupFramebuffers();
		setupFramebufferDraw();
		setupBaseMatrices(); //will need to move/redo to deal with moving camera
		setupCube(); //remove this
	}

	void cleanupGLOnThread()
	{
		//TODO cleanup all the GL gunk
	}

	void cleanupStructuresOnThread()
	{
		//delete data structures
		delete(_textureLoadQueue_p);
		delete(_modelLoadQueue_p);		
		delete(_textures_p);
		delete(_models_p);
	}

	/// <summary>
	/// Checks the queue and grabs new state information
	/// </summary>
	void checkQueue()
	{
		//lock mutex...
		_mqMutex_p->lock();

		//needs to be sensitive to current state and prioritize certain messages
		//REMEMBER: the "head" of the queue is at 0 and the "tail" at the other end
				
		if (!_mq_p->empty()) //optimization: skip EVERYTHING if empty
		{
			if (_state == RendererState::idle)
			{
				auto iter = _mq_p->begin();

				//if nothing is loaded, wait for a load call and delete everything else
				for (; iter != _mq_p->end(); iter++)
				{
					std::shared_ptr<Message> msg_sp = *iter;
					if (msg_sp.get()->getType() == RenderLoadMessageType)
					{
						//great, we can load, so load and break!
						startLoad(static_cast<RenderLoadMessageContent*>(msg_sp.get()->getContent())->data);
						iter = _mq_p->erase(iter); //don't forget to delete that!
						break;
					}
					else
					{
						//remove and log warning
						SDL_Log("Renderer: found a message before load");
						iter = _mq_p->erase(iter);
					}
				}
			}
			else if (_state == RendererState::loading)
			{
				//we won't do anything until the load is done- not even abort!
			}
			else if (_state == RendererState::rendering)
			{
				//if rendering:
				//	if we have an unload message, ignore other messages, start unload and purge everything before unload
				//	if we have a load message, push to "immediate load" queue and continue rendering as normal
				//  if we don't have either, simply do a render (grab latest render scene and render overlay messages)

				//algorithm:
				//traverse from start to finish:
				//store the latest render message and index
				//store the latest renderOverlay message and index
				//if we hit an unload instruction, store that and break
				//if we hit a loadsingle instruction, queue it and continue
				//if we hit a load instruction, log warning, start unload, store index, and break
				RenderableScene *latestScene;
				int latestSceneIndex = 0;
				RenderableOverlay *latestOverlay;
				int latestOverlayIndex = 0;
				int abortIndex = -1;

				for (int i = 0; i < _mq_p->size; i++)
				{
					std::shared_ptr<Message> msg_sp = _mq_p->at(i);
					MESSAGE_TYPE t = msg_sp->getType();

					//DO NOT CHANGE THIS TO A SWITCH
					if (t == RenderDrawMessageType)
					{
						RenderableScene *scn = static_cast<RenderDrawMessageContent*>(msg_sp.get()->getContent())->scene_p;
						latestScene = scn;
						latestSceneIndex = i;
					}
					else if (t == RenderDrawOverlayMessageType)
					{
						RenderableOverlay *ovl = static_cast<RenderDrawOverlayMessageContent*>(msg_sp.get()->getContent())->overlay_p;
						latestOverlay = ovl;
						latestOverlayIndex = i;
					}
					else if (t == RenderUnloadMessageType)
					{
						abortIndex = i;
						break;
					}
					else if (t == RenderLoadSingleMessageType)
					{
						RenderLoadSingleMessageContent smc = *static_cast<RenderLoadSingleMessageContent*>(msg_sp.get()->getContent());
						if (!smc.model.name.empty())
						{
							ModelLoadingData mld;
							mld.name = smc.model.name;
							mld.path = MODEL_BASEPATH_CONST + mld.name + MODEL_EXTENSION_CONST;
							mld.relative = true;
							_modelLoadQueue_p->push_back(mld);
						}
						if (!smc.texture.name.empty())
						{
							TextureLoadingData tld;
							tld.name = smc.texture.name;
							tld.path = TEXTURE_BASEPATH_CONST + tld.name + TEXTURE_EXTENSION_CONST;
							tld.relative = true;
							_textureLoadQueue_p->push_back(tld);
						}
					}
					else if (t == RenderLoadMessageType)
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Received load message while scene is loaded!");
						abortIndex = i - 1;
					}
					else
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Bad message type received!");
					}
				}
				
				if (abortIndex > 0)
				{
					//if abortIndex is a thing, purge everything up and including abortIndex
					startUnload();
					_mq_p->erase(_mq_p->begin(), _mq_p->begin() + abortIndex+1);

				}
				else
				{
					//if renderOverlay comes after render, delete it first, otherwise it'll get deleted normally
					//clear everything up to and including stored index
					if (latestOverlayIndex > latestSceneIndex)
					{
						_mq_p->erase(_mq_p->begin() + latestOverlayIndex);
					}
					_mq_p->erase(_mq_p->begin(), _mq_p->begin()+latestSceneIndex+1);
				}
				
			}
			else if (_state == RendererState::unloading)
			{
				//like loading, we won't do anything if unloading
			}
			else
			{
				//well, that shouldn't happen
			}
		}

		//if currently loading or unloading, ignore messages

		//unlock mutexs
		_mqMutex_p->unlock();
	}


	void startLoad(RenderableSetupData data)
	{
		//some of the doLoad stuff will have to move in here
		//this runs once while doLoad runs every frame until loading is complete
	}

	void startUnload()
	{
		
	}

	/// <summary>
	/// Loads stuff
	/// </summary>
	void doLoad()
	{
		//TODO EVENTUALLY: blit render load screen

		//if we don't have context, get context

		//loads stuff

		//TODO figure this out, probably going to need two queues
	}

	void doSingleLoad()
	{
		//load one thing during drawing process
	}

	/// <summary>
	/// Unloads stuff
	/// </summary>
	void doUnload()
	{
		//TODO EVENTUALLY: blit render load screen

		//delete (some) GL stuff, purge data structures, DO NOT PURGE QUEUE

		//finally release context
	}

	/// <summary>
	/// Renders a frame
	/// </summary>
	void doRender()
	{
		//temporary
		updateCube();

		drawCube();

		//will remain in final
		drawObjects();
		drawLighting();
		drawOverlay();

		SDL_GL_SwapWindow(_window_p);
	}

	void setupWindow()
	{
		//eventually this may do more

		int width, height;
		if (GlobalPrefs::renderHeight > 0 && GlobalPrefs::renderWidth > 0)
		{
			width = GlobalPrefs::renderWidth;
			height = GlobalPrefs::renderHeight;
		}
		else
		{
			SDL_GL_GetDrawableSize(_window_p, &width, &height);
		}

		_renderWidth = width;
		_renderHeight = height;
	}

	void setupProgram()
	{
		_programID = LoadShaders();
	}

	void setupCube()
	{
		glGenVertexArrays(1, &_cubeVertexArrayID);
		glBindVertexArray(_cubeVertexArrayID);

		glGenBuffers(1, &_cubeVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexBufferID);

		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void setupBaseMatrices()
	{
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)_renderWidth / (float)_renderHeight, 0.1f, 100.0f); //TODO deal with near/far plane
		glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 model = glm::mat4(1.0f);
		_baseModelViewMatrix = view * model;
		_baseModelViewProjectionMatrix = projection * view * model;

		_shaderMVPMatrixID = glGetUniformLocation(_programID, "MVP");
	}

	void setupFramebuffers()
	{

		//gen FBO
		glGenFramebuffers(1, &_framebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);

		//gen framebuffer texture
		glGenTextures(1, &_framebufferTextureID);
		glBindTexture(GL_TEXTURE_2D, _framebufferTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _renderWidth, _renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//gen depthbuffer
		glGenRenderbuffers(1, &_renderbufferDepthID);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferDepthID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _renderWidth, _renderHeight);
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

	void setupFramebufferDraw()
	{
		glGenVertexArrays(1, &_framebufferDrawVertexArrayID);
		glBindVertexArray(_framebufferDrawVertexArrayID);

		glGenBuffers(1, &_framebufferDrawVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _framebufferDrawVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		_framebufferDrawProgramID = LoadShadersFBDraw();

	}

	void drawCube()
	{

		//set shader program
		glUseProgram(_programID);

		//bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
		glViewport(0, 0, _renderWidth, _renderHeight);

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

	void drawObjects()
	{
		//TODO draw objects
	}

	void drawObject()
	{
		//TODO draw one object

		//NOTE: should always be tolerant of missing resources!
	}

	void drawLighting()
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

	void drawOverlay()
	{
		//TODO draw overlay
	}

	void updateCube()
	{
		_cubeAngle += CUBE_ROTATE_STEP_CONST;
		glm::mat4 identity = glm::mat4();
		glm::mat4 rotate = glm::rotate(identity, _cubeAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		_cubeModelViewMatrix = rotate;
	}

};

RenderEngine::RenderEngine()
{
	_impl = new RenderEngineImplementation();
}

void RenderEngine::start()
{
	_impl->start();
}

void RenderEngine::update()
{
	_impl->update();
}

RenderEngine::~RenderEngine()
{
	delete(_impl);
}
