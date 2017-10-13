#include "FileEngine.h"
#include "Message.h"
#include "MessageTypes.h"
#include "FileHelper.h"
#include "MessagingSystem.h"

FileEngine::FileEngine()
{
	//we do all the hard work in start()
}

void FileEngine::start()
{
	//subscribe to messages
	subscribe(MESSAGE_TYPE::FileLoadMessageType);

	//start loop
	_thread_p = new std::thread(&FileEngine::loop, this);
}

FileEngine::~FileEngine()
{
	_isRunning = false;
	_thread_p->join();
	delete(_thread_p);

	unsubscribe(MESSAGE_TYPE::FileLoadMessageType);
}

void FileEngine::loop()
{
	//do setup on thread

	while (_isRunning)
	{
		for (int i = 0; i < CYCLE_MESSAGES_CONST; i++)
		{
			if (!_urgentMessageQueue.empty())
			{
				//handle urgent messages first
				_urgentMessageQueueMutex_p->lock();
				Message *msg = _urgentMessageQueue.front().get();
				HandleMessage(msg->getContent());
				_urgentMessageQueue.pop();
				_urgentMessageQueueMutex_p->unlock();

			}
			else if (!_messageQueue.empty())
			{
				//then non-urgent messages
				_messageQueueMutex_p->lock();
				Message *msg = _messageQueue.front().get();
				HandleMessage(msg->getContent());
				_messageQueue.pop();
				_messageQueueMutex_p->unlock();
			}
			else
			{
				//all empty, break
				break;
			}
		}		

		//avoid busywaiting!
		std::this_thread::sleep_for(std::chrono::milliseconds(CYCLE_DELAY_MS_CONST));
	}

	//do teardown on thread

}

void FileEngine::HandleMessage(BaseMessageContent *inBaseMessage)
{
	FileLoadMessageContent inMessage = *static_cast<FileLoadMessageContent*>(inBaseMessage); //this seems safe

	size_t hash = 0;
	std::string content = std::string();

	if (inMessage.relative)
	{
		hash = HashFilePath(inMessage.path, true);
		content = FileHelper::loadFileFromStringRelative(inMessage.path);
	}
	else
	{
		hash = HashFilePath(inMessage.path, false);
		content = FileHelper::loadFileFromString(inMessage.path);
	}

	FileLoadedMessageContent *outMessage = new FileLoadedMessageContent();

	outMessage->hash = hash;
	outMessage->content = content;
	outMessage->path = inMessage.path;
	outMessage->relative = inMessage.relative;

	MessagingSystem::instance().postMessage(std::make_shared<Message>(Message(MESSAGE_TYPE::FileLoadedMessageType,false,outMessage)));
}

size_t FileEngine::HashFilePath(std::string path, bool relative)
{
	std::string prefix;
	if (relative)
		prefix = "R|";
	else
		prefix = "A|";

	std::string strToHash = prefix + path;

	return std::hash<std::string>{}(strToHash);
}
