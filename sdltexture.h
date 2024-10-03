#pragma once

#include <SDL.h>
#include "sdlwrappers.h"

class sdltexture {
private:
	TextureWrapper _t;
	SDL_Rect _rect;
public:
	sdltexture(TextureWrapper t, int w, int h);
	void setPos(int x, int y);
	void render(SDL_Renderer* renderer);
};
