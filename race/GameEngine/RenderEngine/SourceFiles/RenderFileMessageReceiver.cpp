#include "RenderFileMessageReceiver.h"

RenderFileMessageReceiver::RenderFileMessageReceiver(std::vector<std::shared_ptr<Message>> *mq_p)
{
	_mq_p = mq_p;
	_mqMutex_p = new std::mutex();
}

void RenderFileMessageReceiver::subscribeAll()
{
	this->subscribe(FileLoadedMessageType);
}

bool RenderFileMessageReceiver::messageHandler(std::shared_ptr<Message> message)
{
	_mqMutex_p->lock();
	_mq_p->push_back(message);
	_mqMutex_p->unlock();

	return false;
}