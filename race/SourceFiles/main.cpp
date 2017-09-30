#include "main.h" 
#include "Engine.h"
#include <SDL_gamecontroller.h>
#include "InputEngine.h"

uint32_t TICKS_TO_WAIT = 17;
SDL_Window *g_window_p;
SDL_GLContext g_context;

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
	g_window_p = SDL_CreateWindow("RACE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	//g_context = SDL_GL_CreateContext(g_window_p);
	//SDL_GL_MakeCurrent(g_window_p, NULL);

	// game code eventually goes here

	SDL_Log("It worked!");

	//it doesn't start Engine on a separate thread yet; Spencer I'll let you do that
	Engine *e = new Engine();
	e->start();

	//*****temporary loop stolen from racerender

	bool quit = false;
	SDL_Event ev;

	uint32_t ticksAtLast = SDL_GetTicks();

	while (!quit)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_CONTROLLERBUTTONDOWN:
					IE->buttonEventHandler(ev);
					break;
				case SDL_CONTROLLERAXISMOTION:
					IE->axisEventHandler(ev);
					break;
				default:
					break;
			}
		}

		//run the renderer every tick
		uint32_t ticksSinceLast = SDL_GetTicks() - ticksAtLast;
		if (ticksSinceLast >= TICKS_TO_WAIT)
		{
			e->update();
		}
	}

	SDL_DestroyWindow(g_window_p);

	//*****temporary loop section ends

	delete(e);

	SDL_Quit();

	return 0;
}