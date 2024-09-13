#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

class sdlapp {
public:
	sdlapp();
	TTF_Font* loadFont(const char *path, int fontSize) const;
	~sdlapp();
};