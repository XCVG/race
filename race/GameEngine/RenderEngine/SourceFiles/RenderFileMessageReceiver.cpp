#include "RenderFileMessageReceiver.h"
#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif

RenderFileMessageReceiver::RenderFileMessageReceiver(std::vector<std::shared_ptr<Message>> *mq_p, std::mutex *mutex_p)
{
	_mq_p = mq_p;
	_mqMutex_p = mutex_p;
}

void RenderFileMessageReceiver::subscribeAll()
{
	this->subscribe(MESSAGE_TYPE::FileLoadedMessageType);
}

bool RenderFileMessageReceiver::messageHandler(std::shared_ptr<Message> message)
{
	SDL_Log("Renderer: Received a file message");

	_mqMutex_p->lock();
	_mq_p->push_back(message);
	_mqMutex_p->unlock();

	return false;
}