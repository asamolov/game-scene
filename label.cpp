#include "label.h"
#include "sdlwrappers.h"

#include <stdexcept>
#include <fmt/core.h>

label::label(TTF_Font* font): font(font), _rect({0, 0, 0, 0})
{
}

void label::setPos(int x, int y)
{
	_rect.x = x;
	_rect.y = y;
}

void label::render(SDL_Renderer* renderer)
{
	if (text.empty()) {
		// not rendering empty strings
		return;
	}
	// TODO: cache the texture if the text is not changed
	SurfaceWrapper textSurface(TTF_RenderText_Solid(font, text.c_str(), SDL_Color{0x00, 0x00, 0x00}));
	TextureWrapper texture(SDL_CreateTextureFromSurface(renderer, textSurface.get()));
	_rect.h = textSurface->h;
	_rect.w = textSurface->w;
	SDL_RenderCopy(renderer, texture.get(), NULL, &_rect);
}