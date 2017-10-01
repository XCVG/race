#include "RenderMessageReceiver.h"

RenderMessageReceiver::RenderMessageReceiver(std::vector<std::shared_ptr<Message>> *mq_p)
{
	_mq_p = mq_p;
	_mqMutex_p = new std::mutex();
}

void RenderMessageReceiver::subscribeAll()
{
	this->subscribe(RenderLoadMessageType);
	this->subscribe(RenderLoadSingleMessageType);
	this->subscribe(RenderDrawMessageType);
	this->subscribe(RenderDrawOverlayMessageType);
	this->subscribe(RenderUnloadMessageType);
}

bool RenderMessageReceiver::messageHandler(std::shared_ptr<Message> message)
{
	_mqMutex_p->lock();
	_mq_p->push_back(message);
	_mqMutex_p->unlock();

	return false;
}