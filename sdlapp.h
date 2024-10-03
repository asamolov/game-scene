#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class sdlapp {
public:
	sdlapp();
	TTF_Font* loadFont(const char* path, int fontSize) const;
	SDL_Surface* LoadImage(const std::string& path) const;
	~sdlapp();
};