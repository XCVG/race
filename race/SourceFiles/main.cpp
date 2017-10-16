#include "main.h" 
#include "Engine.h"
#include "GlobalPrefs.h"
#include "InputEngine.h"
#include "MessagingSystem.h"

SDL_Window *g_window_p;
SDL_GLContext g_context;
std::thread* engineThread_p;
SDL_GameController *gameController;
const int CONTROLLER_DEADZONE = 8000;
/// <summary>
/// Application entry point
/// </summary>
/// <param name="argc">Number of arguments passed to the application</param>
/// <param name="argv">Array containg string arguments passed to the application</param>
/// <return>Status code on application exit.</return>
int main(int argc, char ** argv) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

	InputEngine *IE = new InputEngine();

	//open opengl and window
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
	g_window_p = SDL_CreateWindow("RACE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GlobalPrefs::windowWidth, GlobalPrefs::windowHeight, SDL_WINDOW_OPENGL);
	//g_context = SDL_GL_CreateContext(g_window_p);
	//SDL_GL_MakeCurrent(g_window_p, NULL);

	// game code eventually goes here

	SDL_Log("It worked!");

	//it doesn't start Engine on a separate thread yet; Spencer I'll let you do that
	MessagingSystem::instance().start();
	Engine *e = new Engine();
	engineThread_p = e->start();

	//*****temporary loop stolen from racerender

	bool quit = false;
	SDL_Event ev;

	for (int x = 0; x < SDL_NumJoysticks(); x++)
	{
		if (SDL_IsGameController(x))
		{
			gameController = SDL_GameControllerOpen(x);
			break;
		}
	}
	

	uint32_t ticksAtLast = SDL_GetTicks();

	while (!quit)
	{
		SDL_PollEvent(&ev);
		switch (ev.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
		}
		Sint16 xLook = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX);
		Sint16 yLook = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY);
		if (xLook < CONTROLLER_DEADZONE && xLook > -CONTROLLER_DEADZONE)
			xLook = 0;

		if (yLook < CONTROLLER_DEADZONE && yLook > -CONTROLLER_DEADZONE)
			yLook = 0;

		if (yLook != 0 || xLook != 0) 
			IE->lookEventHandler(xLook, yLook);

		//run the renderer every tick
		/*uint32_t ticksSinceLast = SDL_GetTicks() - ticksAtLast;
		if (ticksSinceLast >= TICKS_TO_WAIT)
		{
			//e->update();
		}*/
	}

	if (gameController != NULL)
	{
		SDL_GameControllerClose(gameController);
	}

	

	//SDL_JoystickClose(joystick);

	SDL_Log("Main::Out of Loop");
	e->flagLoop();
	SDL_Log("Main::Wait for Engine Join");
	engineThread_p->join();
	SDL_Log("Engine::Thread Join");
	delete(e);
	delete(IE);
	MessagingSystem::instance().kill();
	SDL_DestroyWindow(g_window_p);

	//*****temporary loop section ends

	SDL_Quit();

	return 0;
}