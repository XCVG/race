#include "SoundEngine.h"

SoundEngine::SoundEngine()
{
	subscribe(MESSAGE_TYPE::SoundMessageType);
	SDL_Init(SDL_INIT_AUDIO);

	basePath = std::string(SDL_GetBasePath()) + "ResourceFiles/Sounds/";

	// Check frequency (22050 / 44100) and chunksize (2048 / 4096) values to adjust in case of sound lag.
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		SDL_Log("Error: %s", Mix_GetError(), "\n");
	}

	//TODO dynamic loading
	gameSoundtracks.emplace("testmusic", Mix_LoadMUS((basePath + "Music.mp3").c_str()));
	gameSoundEffects.emplace("testsfx", Mix_LoadWAV((basePath + "SEffect.wav").c_str()));
}

SoundEngine::~SoundEngine() 
{
	Mix_FreeChunk(gameSoundEffects.at("testsfx"));
	Mix_FreeMusic(gameSoundtracks.at("testmusic"));

	Mix_Quit();
	SDL_Quit();
}

void SoundEngine::loop()
{
	if (!_urgentMessageQueue.empty())
	{
		//handle urgent messages first
		_urgentMessageQueueMutex_p->lock();
		std::shared_ptr<Message> msg = _urgentMessageQueue.front();
		_urgentMessageQueue.pop();
		_urgentMessageQueueMutex_p->unlock();
		HandleMessage(msg);
	}
	else if (!_messageQueue.empty())
	{
		//then non-urgent messages
		_messageQueueMutex_p->lock();
		std::shared_ptr<Message> msg = _messageQueue.front();
		_messageQueue.pop();
		_messageQueueMutex_p->unlock();
		HandleMessage(msg);
	}
}

void SoundEngine::HandleMessage(std::shared_ptr<Message> inBaseMessage)
{
	MESSAGE_TYPE contentType = inBaseMessage->getType();

	if (contentType == MESSAGE_TYPE::SoundMessageType)
	{
		SoundMessageContent inMessage = *static_cast<SoundMessageContent*>(inBaseMessage->getContent());
		SoundMessage(inMessage);
	}
}

void SoundEngine::SoundMessage(SoundMessageContent inMessageContent)
{
	switch (inMessageContent.subType)
	{
		case S_TYPE::playMusic:
		{
			if (!Mix_PlayingMusic())
			{
				if(gameSoundtracks.count(inMessageContent.name) > 0)
					Mix_PlayMusic(gameSoundtracks.at(inMessageContent.name), -1);
			}
			else if (Mix_PausedMusic())
			{
				Mix_ResumeMusic();
			}
			else
			{
				Mix_PauseMusic();
			}
			break;
		}

		case S_TYPE::stopMusic:
		{
				Mix_HaltMusic();
				break;
		}

		case S_TYPE::playSound:
		{
			if (gameSoundEffects.count(inMessageContent.name) > 0)
				Mix_PlayChannel(-1, gameSoundEffects.at(inMessageContent.name), 0);
			break;
		}
	}
}
