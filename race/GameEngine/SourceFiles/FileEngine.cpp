#include "FileEngine.h"
#include "FileHelper.h"

FileEngine::FileEngine()
{
	//constructor: on engine thread
}

void FileEngine::start()
{
	//start: on engine thread

	//TODO subscribe to messaging

	//spawn thread
	_isRunning = true;
	_filethread_p = new std::thread(&FileEngine::loop, this);
}

void FileEngine::update()
{
	//update: on engine thread
}

void FileEngine::loop()
{
	//loop: on fileengine thread

	SDL_Log("FileEngine thread started!");

	//do any thread-specific setup

	//loop
	while (_isRunning)
	{
		//TODO serve file requests
	}

	//do any thread-specific cleanup
	SDL_Log("FileEngine thread halted!");
}

FileEngine::~FileEngine()
{
	//destructor: on engine thread

	_isRunning = false;
	_filethread_p->join();
	delete(_filethread_p);
}
