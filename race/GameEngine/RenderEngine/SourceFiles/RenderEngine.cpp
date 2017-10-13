#ifdef __APPLE__
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <SDL.h>
#endif

#include <thread>
#include <queue>
#include <atomic>

#include "RenderEngine.h"

#include "GlobalPrefs.h"
#include "FileEngine.h"
#include "OBJImport.h"

#include "RenderableTypes.h"
#include "RendererInternalTypes.h"
#include "RenderMessageReceiver.h"
#include "RenderFileMessageReceiver.h"

#include "MessagingSystem.h"
#include "MessageTypes.h"
#include "MessageReceiver.h"
#include "Message.h"

#include "Shaders.h"
#include "Quad.h"
#include "Cube.h"
#include "main.h"

const int_least64_t IDLE_DELAY_CONST = 10;
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
	std::vector<ModelLoadingData> *_modelAwaitQueue_p;
	std::vector<TextureLoadingData> *_textureAwaitQueue_p;

	//threading stuff
	bool _isRunning;
	std::thread *_renderThread_p;

	//honestly not sure
	GLuint _programID = 0;

	//framebuffer stuff
	int _renderWidth = 0;
	int _renderHeight = 0;

	GLuint _framebufferID = 0;
	GLuint _framebufferTextureID = 0;
	GLuint _renderbufferDepthID = 0;

	GLuint _framebufferDrawProgramID = 0;
	GLuint _framebufferDrawVertexArrayID = 0;
	GLuint _framebufferDrawVertexBufferID = 0;
	GLuint _framebufferDrawTexID = 0;

	//temporary cube stuff
	GLuint _cubeVertexArrayID = 0;
	GLuint _cubeVertexBufferID = 0;
	glm::mat4 _cubeModelViewMatrix;
	float _cubeAngle;

	//base MVP, may keep or remove
	GLuint _shaderMVPMatrixID = 0;
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
		SDL_Log("%s", std::to_string(SDL_GL_GetSwapInterval()).c_str());
		_state = RendererState::idle;

		//loop: on RenderEngine thread
		while (_isRunning)
		{
			//TODO: state switching and stuff
			//doLoad, doRender/doImmediateLoad, doUnload

			checkQueue();

			switch ((RendererState)_state)
			{
			case RendererState::idle:
				//SDL_Log("Idle");
				doIdle();
				std::this_thread::sleep_for(std::chrono::milliseconds(IDLE_DELAY_CONST)); //don't busywait!
				break;
			case RendererState::loading:
				//SDL_Log("Loading");
				doLoad();
				break;
			case RendererState::rendering:
				//SDL_Log("Rendering");
				doSingleLoad();
				doRender(); //this should run really absurdly fast
				break;
			case RendererState::unloading:
				//SDL_Log("Unloading");
				doUnload();
				break;
			}

			//std::this_thread::sleep_for(std::chrono::milliseconds(17));
		}

		//force unload/release if we're not already unloaded
		if (_state != RendererState::idle)
		{
			startUnload();
			doUnload();
		}

		//cleanup after run
		cleanupSceneOnThread();
		cleanupGLOnThread();
		cleanupStructuresOnThread();

		SDL_Log("RenderEngine thread halted!");
	}

	void setupStructuresOnThread()
	{
		//setup data structures

		_models_p = new std::map<std::string, ModelData>();
		_textures_p = new std::map<std::string, TextureData>();
		_modelLoadQueue_p = new std::vector<ModelLoadingData>();
		_textureLoadQueue_p = new std::vector<TextureLoadingData>();
		_modelAwaitQueue_p = new std::vector<ModelLoadingData>();
		_textureAwaitQueue_p = new std::vector<TextureLoadingData>();
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
		//setupBaseMatrices(); //will need to move/redo to deal with moving camera
		setupCube(); //remove this soon
	}

	void cleanupSceneOnThread()
	{
		cleanupProgram();
		cleanupFramebuffers();
		cleanupFramebufferDraw();
	}

	void cleanupGLOnThread()
	{
		//TODO cleanup all the GL gunk
	}

	void cleanupStructuresOnThread()
	{
		//delete data structures
		delete(_textureAwaitQueue_p);
		delete(_modelAwaitQueue_p);
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
					if (msg_sp.get()->getType() == MESSAGE_TYPE::RenderLoadMessageType)
					{
						//great, we can load, so load and break!
						startLoad(&static_cast<RenderLoadMessageContent*>(msg_sp.get()->getContent())->data);
						iter = _mq_p->erase(iter); //don't forget to delete that!
						break;
					}
					else
					{
						//remove and log warning
						SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: found a message before load");
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
				RenderableScene *latestScene = nullptr;
				int latestSceneIndex = 0;
				RenderableOverlay *latestOverlay = nullptr;
				int latestOverlayIndex = 0;
				int abortIndex = -1;

				for (int i = 0; i < _mq_p->size(); i++)
				{
					std::shared_ptr<Message> msg_sp = _mq_p->at(i);
					MESSAGE_TYPE t = msg_sp->getType();

					//DO NOT CHANGE THIS TO A SWITCH
					if (t == MESSAGE_TYPE::RenderDrawMessageType)
					{
						RenderableScene *scn = static_cast<RenderDrawMessageContent*>(msg_sp.get()->getContent())->scene_p;
						latestScene = scn;
						latestSceneIndex = i;
					}
					else if (t == MESSAGE_TYPE::RenderDrawOverlayMessageType)
					{
						RenderableOverlay *ovl = static_cast<RenderDrawOverlayMessageContent*>(msg_sp.get()->getContent())->overlay_p;
						latestOverlay = ovl;
						latestOverlayIndex = i;
					}
					else if (t == MESSAGE_TYPE::RenderUnloadMessageType)
					{
						abortIndex = i;
						break;
					}
					else if (t == MESSAGE_TYPE::RenderLoadSingleMessageType)
					{
						RenderLoadSingleMessageContent smc = *static_cast<RenderLoadSingleMessageContent*>(msg_sp.get()->getContent());
						if (!smc.model.name.empty())
						{
							ModelLoadingData mld;
							mld.name = smc.model.name;
							_modelLoadQueue_p->push_back(mld);
						}
						if (!smc.texture.name.empty())
						{
							TextureLoadingData tld;
							tld.name = smc.texture.name;
							_textureLoadQueue_p->push_back(tld);
						}
					}
					else if (t == MESSAGE_TYPE::RenderLoadMessageType)
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: Received load message while scene is loaded!");
						abortIndex = i - 1;
					}
					else
					{
						SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: Bad message type received!");
					}
				}

				if (abortIndex > 0)
				{
					//if abortIndex is a thing, purge everything up and including abortIndex					
					_mq_p->erase(_mq_p->begin(), _mq_p->begin() + abortIndex + 1);
					startUnload();
				}
				else
				{
					//if renderOverlay comes after render, delete it first, otherwise it'll get deleted normally
					//clear everything up to and including stored index
					if (latestOverlayIndex > latestSceneIndex)
					{
						_mq_p->erase(_mq_p->begin() + latestOverlayIndex);
					}
					_mq_p->erase(_mq_p->begin(), _mq_p->begin() + latestSceneIndex + 1);

					//assign renderablescene and renderableoverlay if they exist
					if (latestScene != nullptr)
					{
						if (_lastScene_p == nullptr)
							delete(_lastScene_p);
						_lastScene_p = latestScene;
					}
					if (latestOverlay != nullptr)
					{
						if (_lastOverlay_p == nullptr)
							delete(_lastOverlay_p);
						_lastOverlay_p = latestOverlay;
					}
				}

			}
			else if (_state == RendererState::unloading)
			{
				//like loading, we won't do anything if unloading
			}
			else
			{
				//well, that shouldn't happen
				SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: Ended up in an impossible state");
			}
		}

		//if currently loading or unloading, ignore messages

		//unlock mutexs
		_mqMutex_p->unlock();
	}


	void startLoad(RenderableSetupData *data)
	{
		SDL_Log("Renderer: Entering load state");

		_state = RendererState::loading;

		//just start the load here
		for (auto mdata : data->models)
		{
			if (!mdata.empty())
			{
				ModelLoadingData mld;
				mld.name = mdata;
				_modelLoadQueue_p->push_back(mld);
			}
		}

		for (auto tdata : data->textures)
		{
			if (!tdata.empty())
			{
				TextureLoadingData tld;
				tld.name = tdata;
				_textureLoadQueue_p->push_back(tld);
			}
		}

		//grab context
		acquireContext();

		//DON'T DO ANYTHING LONG IN HERE BECAUSE THE QUEUE IS STILL BLOCKED
	}

	void startUnload()
	{
		_state = RendererState::unloading;
	}

	/// <summary>
	/// Loads stuff
	/// </summary>
	void doLoad()
	{
		//needs refactoring badly

		//possible future optimization: do things you don't need context for when you don't have context

		//if we don't have context, get context
		if (!haveContext())
			acquireContext();
		if (!haveContext())
			return; //wait for context to be released

		//TODO EVENTUALLY: blit render load screen
		drawLoadScreen();

		//loads stuff

		MessagingSystem *ms = &MessagingSystem::instance();
		if (ms == nullptr)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Renderer: messaging system does not exist");
			return;
		}

		//possible future optimization: don't push the entire queues at once
		//dispatch from model and texture load queues
		if (!_modelLoadQueue_p->empty())
		{
			for (int i = 0; i < _modelLoadQueue_p->size(); i++)
			{
				ModelLoadingData mld = _modelLoadQueue_p->at(i);

				FileLoadMessageContent *flmc = new FileLoadMessageContent();
				flmc->path = MODEL_BASEPATH_CONST + mld.name + MODEL_EXTENSION_CONST;
				flmc->relative = true;

				mld.hash = FileEngine::HashFilePath(flmc->path, flmc->relative);

				Message *msg = new Message(MESSAGE_TYPE::FileLoadMessageType, false, flmc);
				ms->postMessage(std::shared_ptr<Message>(msg));

				_modelAwaitQueue_p->push_back(mld);
			}

			_modelLoadQueue_p->clear();
		}

		if (!_textureLoadQueue_p->empty())
		{
			for (int i = 0; i < _textureLoadQueue_p->size(); i++)
			{
				TextureLoadingData tld = _textureLoadQueue_p->at(i);

				FileLoadMessageContent *flmc = new FileLoadMessageContent();
				flmc->path = TEXTURE_BASEPATH_CONST + tld.name + TEXTURE_EXTENSION_CONST;
				flmc->relative = true;

				tld.hash = FileEngine::HashFilePath(flmc->path, flmc->relative);

				Message *msg = new Message(MESSAGE_TYPE::FileLoadMessageType, false, flmc);
				ms->postMessage(std::shared_ptr<Message>(msg));

				_textureAwaitQueue_p->push_back(tld);
			}

			_textureAwaitQueue_p->clear();
		}

		//process results from file return queue
		//not optimized for concurrency TODO
		if (!_fmq_p->empty())
		{
			_fmqMutex_p->lock();

			for (int i = 0; i < _fmq_p->size(); i++)
			{
				Message *msg = _fmq_p->at(i).get();
				BaseMessageContent * bmc = msg->getContent();
				FileLoadedMessageContent *flmc = static_cast<FileLoadedMessageContent*>(bmc);

				int64_t foundModel = -1;
				ModelLoadingData foundMLD;
				int64_t foundTexture = -1;
				TextureLoadingData foundTLD;

				for (int j = 0; j < _modelAwaitQueue_p->size(); j++)
				{
					ModelLoadingData mld = _modelAwaitQueue_p->at(j);
					if (mld.hash == flmc->hash)
					{
						foundModel = j;
						break;
					}
				}

				if (foundModel >= 0)
				{
					loadOneModel(foundMLD, &flmc->content);
					_modelAwaitQueue_p->erase(_modelAwaitQueue_p->begin() + foundModel);
					continue;
				}

				for (int j = 0; j < _textureAwaitQueue_p->size(); j++)
				{
					TextureLoadingData tld = _textureAwaitQueue_p->at(j);
					if (tld.hash == flmc->hash)
					{
						foundTexture = j;
						break;
					}
				}

				if (foundTexture >= 0)
				{
					loadOneTexture(foundTLD, &flmc->content);
					_textureAwaitQueue_p->erase(_textureAwaitQueue_p->begin() + foundTexture);
				}
			}

			_fmq_p->clear();

			_fmqMutex_p->unlock();
		}

		//loading is done if and only if both load and await queues are empty and we have context
		if (_textureLoadQueue_p->empty() && _textureAwaitQueue_p->empty() && _modelLoadQueue_p->empty() && _modelAwaitQueue_p->empty() && haveContext())
		{
			ms->postMessage(std::shared_ptr<Message>(new Message(MESSAGE_TYPE::RenderReadyMessageType,false,new RenderReadyMessageContent())));
			SDL_Log("Renderer: Entering render state");
			_state = RendererState::rendering;
		}

	}

	void doSingleLoad()
	{
		//load one thing during drawing process

		//dispatch ONE model and ONE texture from load queues (if nonempty)

		//check the file return queue for ONE file
	}

	void loadOneModel(ModelLoadingData mld, std::string *data_p)
	{

		//return;

		ModelData md;

		//does nothing yet
		auto objData = OBJImport::importObjInfo(*data_p);
		GLuint numVertices = objData.size() / 8;

		GLfloat *objPtr = &objData[0];
		GLuint glVaoId, glVboId;

		glGenVertexArrays(1, &glVaoId);
		glBindVertexArray(glVaoId);
		glGenBuffers(1, &glVboId);
		glBindBuffer(GL_ARRAY_BUFFER, glVboId);
		glBufferData(GL_ARRAY_BUFFER, (objData.size() * sizeof(GLfloat)), objPtr, GL_STATIC_DRAW);
		//will break but shouldn't break other things

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		md.numVerts = numVertices;
		md.preScale = 1.0f;
		md.vaoID = glVaoId;
		md.vboID = glVboId;

		_models_p->emplace(mld.name, md);

	}

	void loadOneTexture(TextureLoadingData tld, std::string *data_p)
	{
		//does nothing yet

	}

	/// <summary>
	/// Unloads stuff
	/// </summary>
	void doUnload()
	{
		//TODO EVENTUALLY: blit render load screen
		drawUnloadScreen();

		//delete (some) GL stuff, purge data structures, DO NOT PURGE MESSAGE QUEUES
		unloadGL();
		unloadData();

		//finally release context
		releaseContext();

		_state = RendererState::idle;
	}

	void doIdle()
	{
		//drawIdleScreen();
	}

	void unloadGL()
	{
		//unbind all OGL
	}

	void unloadData()
	{
		//clear (but DO NOT DELETE) data structures
	}

	void drawLoadScreen()
	{
		if (!haveContext())
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glClearColor(0.1f, 0.75f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void drawUnloadScreen()
	{
		if (!haveContext())
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glClearColor(0.1f, 0.25f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void drawIdleScreen()
	{
		if (!haveContext())
			return;

		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glClearColor(0.75f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		_shaderMVPMatrixID = glGetUniformLocation(_programID, "MVP");
	}

	void cleanupProgram()
	{
		//delete shaders/program
		if(_programID > 0)
			glDeleteProgram(_programID);
	}

	void setupCube()
	{
		glGenVertexArrays(1, &_cubeVertexArrayID);
		glBindVertexArray(_cubeVertexArrayID);

		glGenBuffers(1, &_cubeVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void setupBaseMatrices()
	{
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)_renderWidth / (float)_renderHeight, 0.1f, 100.0f); //TODO deal with near/far plane
		glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::mat4 model = glm::mat4(1.0f);
		_baseModelViewMatrix = view * model;
		_baseModelViewProjectionMatrix = projection * view * model;

		//don't delete this, we still need this
		//_shaderMVPMatrixID = glGetUniformLocation(_programID, "MVP");
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

	void cleanupFramebuffers()
	{
		//delete FBOs
		glDeleteRenderbuffers(1, &_renderbufferDepthID);
		glDeleteTextures(1, &_framebufferTextureID);
		glDeleteFramebuffers(1, &_framebufferID);
	}

	void setupFramebufferDraw()
	{
		/*
		glGenBuffers(1, &_cubeVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _cubeVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		glGenVertexArrays(1, &_cubeVertexArrayID);
		glBindVertexArray(_cubeVertexArrayID);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		*/

		glGenVertexArrays(1, &_framebufferDrawVertexArrayID);
		glBindVertexArray(_framebufferDrawVertexArrayID);

		glGenBuffers(1, &_framebufferDrawVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _framebufferDrawVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		_framebufferDrawProgramID = LoadShadersFBDraw();

	}

	void cleanupFramebufferDraw()
	{
		// delete VBOs
		glDeleteBuffers(1, &_framebufferDrawVertexBufferID);
		glDeleteVertexArrays(1, &_framebufferDrawVertexArrayID);
	}

	/// <summary>
	/// Renders a frame
	/// </summary>
	void doRender()
	{
		//temporary
		//updateCube();

		//drawCube();

		//will remain in final
		if (_lastScene_p == nullptr)
		{
			drawNullScene();
		}
		else
		{
			drawCamera(_lastScene_p);
			drawObjects(_lastScene_p);
			//updateCube();
			//drawCube();
			drawLighting(_lastScene_p);
			
		}

		if (_lastOverlay_p == nullptr)
		{
			drawNullOverlay();
		}
		else
		{
			drawOverlay();
		}

		//TODO vsync/no vsync
		SDL_GL_SwapWindow(_window_p);
	}

	void drawNullScene()
	{
		//fallback drawing routine if no scene is available
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int w, h;
		SDL_GL_GetDrawableSize(_window_p, &w, &h);
		glViewport(0, 0, w, h);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	void drawCamera(RenderableScene *scene)
	{
		RenderableCamera *camera = &scene->camera;

		//"draw" the camera, actually just set up base matrices
		
		glm::mat4 projection = glm::perspective(camera->viewAngle, (float)_renderWidth / (float)_renderHeight, camera->nearPlane, camera->farPlane);
		glm::mat4 look = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		glm::mat4 translation = glm::translate(look, camera->position * -1.0f);
		glm::mat4 rotation = glm::eulerAngleYXZ(camera->rotation.y, camera->rotation.x, camera->rotation.z);
		glm::mat4 view = rotation * translation;
		_baseModelViewMatrix = view;
		_baseModelViewProjectionMatrix = projection * view;

	}

	void drawObjects(RenderableScene *scene)
	{

		//bind framebuffer and clear
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //TODO use camera color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//set shader program (here?)
		//glUseProgram(_programID);

		//TODO draw objects
		for (int i = 0; i < scene->objects.size(); i++)
		{
			RenderableObject *ro = &scene->objects[i];
			drawObject(ro);
		}

	}

	void drawObject(RenderableObject *object)
	{
		//TODO draw one arbitraty object
		//NOTE: should always be tolerant of missing resources!
		
		//below: temporary cube code

		//set shader program
		glUseProgram(_programID);

		//bind cube, set properties, and draw
		glBindVertexArray(_cubeVertexArrayID);

		//transform!
		glm::mat4 cubeMVM = glm::mat4();
		cubeMVM = glm::translate(cubeMVM, object->position);
		cubeMVM = glm::scale(cubeMVM, object->scale);
		cubeMVM = glm::rotate(cubeMVM, object->rotation.y, glm::vec3(0, 1, 0));
		cubeMVM = glm::rotate(cubeMVM, object->rotation.x, glm::vec3(1, 0, 0));
		cubeMVM = glm::rotate(cubeMVM, object->rotation.z, glm::vec3(0, 0, 1));
		glm::mat4 cubeMVPM = _baseModelViewProjectionMatrix *  cubeMVM;
		glUniformMatrix4fv(_shaderMVPMatrixID, 1, GL_FALSE, &cubeMVPM[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void drawLighting(RenderableScene *scene)
	{
		//setup framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		int w, h;
		SDL_GL_GetDrawableSize(_window_p, &w, &h);
		glViewport(0, 0, w, h);

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//setup shader
		glUseProgram(_framebufferDrawProgramID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _framebufferTextureID);
		glUniform1i(_framebufferDrawTexID, 0);

		//setup vertices
		glBindVertexArray(_framebufferDrawVertexArrayID);

		//draw
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

	}

	void drawOverlay()
	{
		//TODO draw overlay
	}

	void drawNullOverlay()
	{
		//fallback overlay draw if no overlay is available

	}

	void drawCube()
	{

		//set shader program
		glUseProgram(_programID);

		//bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);		
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, _renderWidth, _renderHeight);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bind cube, set properties, and draw
		glBindVertexArray(_cubeVertexArrayID);		
		//glBindVertexArray(_cubeVertexArrayID);
		//glm::mat4 testView = glm::translate(glm::mat4(), glm::vec3(0,0,-2));
		glm::mat4 cubeMVPM = _baseModelViewProjectionMatrix *  _cubeModelViewMatrix;
		glUniformMatrix4fv(_shaderMVPMatrixID, 1, GL_FALSE, &cubeMVPM[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
	}

	void updateCube()
	{
		_cubeAngle += CUBE_ROTATE_STEP_CONST;
		glm::mat4 identity = glm::mat4();
		glm::mat4 rotate = glm::rotate(identity, _cubeAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		_cubeModelViewMatrix = rotate;
	}

	bool acquireContext()
	{
		if (SDL_GL_GetCurrentContext() == _context_p)
		{
			return true; //we already have it!
		}
		else
		{
			int responseCode = SDL_GL_MakeCurrent(_window_p, _context_p);
			if (responseCode == 0)
				return true;
		}

		return false;
	}

	bool releaseContext()
	{
		//does nothing but we may need it later
		return true;
	}

	bool haveContext()
	{
		return SDL_GL_GetCurrentContext() == _context_p;
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
