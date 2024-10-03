#pragma once

#include <SDL.h>
#include "sdlwrappers.h"

class sdltexture {
private:
	TextureWrapper _t;
	SDL_Rect _rect;
	SDL_Rect _clip;
public:
	sdltexture(sdltexture&& other) noexcept;
	sdltexture& operator=(sdltexture&& other) noexcept;
	sdltexture(TextureWrapper t, int w, int h) noexcept;
	void setPos(int x, int y);
	void setSize(int w, int h);
	void setColorMod(Uint8 r, Uint8 g, Uint8 b);
	void render(SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer, const SDL_Rect & rect);
};
