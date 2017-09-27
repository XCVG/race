#pragma once
#include <thread>
#include <queue>

struct FileMessage
{
	//TODO, temporary placeholder
};

class FileEngine
{
public:
	void start();
	void update();
	FileEngine();
	~FileEngine();
private:
	void loop();

	std::thread *_filethread_p;
	bool _isRunning;

	std::queue<FileMessage> *_messageQueue_p;
};