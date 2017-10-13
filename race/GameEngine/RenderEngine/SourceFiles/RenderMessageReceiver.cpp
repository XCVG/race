#include "RenderMessageReceiver.h"
#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif

RenderMessageReceiver::RenderMessageReceiver(std::vector<std::shared_ptr<Message>> *mq_p)
{
	_mq_p = mq_p;
	_mqMutex_p = new std::mutex();
}

void RenderMessageReceiver::subscribeAll()
{
	this->subscribe(MESSAGE_TYPE::RenderLoadMessageType);
	this->subscribe(MESSAGE_TYPE::RenderLoadSingleMessageType);
	this->subscribe(MESSAGE_TYPE::RenderDrawMessageType);
	this->subscribe(MESSAGE_TYPE::RenderDrawOverlayMessageType);
	this->subscribe(MESSAGE_TYPE::RenderUnloadMessageType);
}

bool RenderMessageReceiver::messageHandler(std::shared_ptr<Message> message)
{
	SDL_Log("Renderer: Received a message!");

	_mqMutex_p->lock();
	_mq_p->push_back(message);
	_mqMutex_p->unlock();

	return false;
}