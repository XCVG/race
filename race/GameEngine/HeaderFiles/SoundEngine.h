#pragma once
#include "Message.h"
#include "MessagingSystem.h"
#include <memory>
#include <SDL.h>
#include <SDL_mixer.h>

class SoundEngine : MessageReceiver {
public:
    SoundEngine();
    ~SoundEngine();
	void loop();

private:
	std::map<std::string, Mix_Music*> gameSoundtracks;
	std::map<std::string, Mix_Chunk*> gameSoundEffects;
	void HandleMessage(std::shared_ptr<Message> inBaseMessage);
	void SoundMessage(SoundMessageContent inMessageContent);
};