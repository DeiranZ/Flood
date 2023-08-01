#pragma once
#include <SDL_stdinc.h>

struct Color {
	Uint8 r;
	Uint8 g;
	Uint8 b;

	Color(Uint8 newR, Uint8 newG, Uint8 newB) :
		r(newR), g(newG), b(newB)
	{

	}

	Color()
	{

	}
};