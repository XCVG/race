#include "main.h" 
#include "Engine.h"

uint32_t TICKS_TO_WAIT = 17;
SDL_Window *g_window_p;

/// <summary>
/// Application entry point
/// </summary>
/// <param name="argc">Number of arguments passed to the application</param>
/// <param name="argv">Array containg string arguments passed to the application</param>
/// <return>Status code on application exit.</return>
int main(int argc, char ** argv) {
	SDL_Init(SDL_INIT_VIDEO);

	//open opengl and window
	g_window_p = SDL_CreateWindow("RACE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

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
			if (ev.type == SDL_QUIT)
			{
				quit = true;
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