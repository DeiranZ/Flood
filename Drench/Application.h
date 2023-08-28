// Thanks to www.studyplan.dev

#pragma once
#include <SDL.h>
#include "Window.h"

class Application
{
public:
	explicit Application(Window* Window) : mWindow(Window)
	{

	}

	SDL_Renderer* GetWindowRenderer()
	{
		return mWindow->GetRenderer();
	}

	void Quit()
	{
		SDL_Event QuitEvent{ SDL_QUIT };
		SDL_PushEvent(&QuitEvent);
	}

private:
	Window* mWindow;
};