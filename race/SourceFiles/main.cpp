#include "main.h" 
#include "Engine.h"
#include <SDL_gamecontroller.h>

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

	// --->

	/*SDL_Joystick *XboxController = SDL_JoystickOpen(0);

	SDL_Log("Controller: %s", SDL_JoystickName(XboxController));
	SDL_Log("Axes: %d", SDL_JoystickNumAxes(XboxController));
	SDL_Log("Buttons: %d", SDL_JoystickNumButtons(XboxController));*/

	// Checking if there is a controller and setting the joystick to the gamecontroller that we are going to use.

	SDL_GameController *gameController = nullptr;

	for (int x = 0; x < SDL_NumJoysticks(); x++)
	{
		if (SDL_IsGameController(x))
		{
			gameController = SDL_GameControllerOpen(x);
			break;
		}
	}

	// <---

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
			if (ev.type == SDL_QUIT)
			{
				quit = true;
			}
			
			else if (ev.type == SDL_CONTROLLERBUTTONDOWN)
			{
				if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_A)
				{
					SDL_Log("A BUTTON PRESSED");
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_B)
				{
					SDL_Log("B BUTTON PRESSED");
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_X)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
				{
					// Do Stuff
				}

				else if (ev.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
				{
					// Do Stuff
				}
			}

			else if (ev.type == SDL_CONTROLLERAXISMOTION)
			{
				if (ev.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
				{
					SDL_Log("LEFT AXIS X MOVED");
				}

				else if (ev.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
				{
					SDL_Log("LEFT AXIS Y MOVED");
				}

				else if (ev.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
				{
					SDL_Log("RIGHT AXIS X MOVED");
				}

				else if (ev.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
				{
					SDL_Log("RIGHT AXIS Y MOVED");
				}

				else if (ev.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
				{
					SDL_Log("LEFT TRIGGER PRESSED");
				}

				else if (ev.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
				{
					SDL_Log("RIGHT TRIGGER PRESSED");
				}
			}

		}

		//run the renderer every tick
		uint32_t ticksSinceLast = SDL_GetTicks() - ticksAtLast;
		if (ticksSinceLast >= TICKS_TO_WAIT)
		{
			e->update();
		}
	}

	// --->

	// Checking If there is no controller and setting controller value back to nullptr.

	if (gameController != NULL)
	{
		SDL_GameControllerClose(gameController);
	}

	gameController = nullptr;

	// <---

	SDL_DestroyWindow(g_window_p);

	//*****temporary loop section ends

	delete(e);

	SDL_Quit();

	return 0;
}