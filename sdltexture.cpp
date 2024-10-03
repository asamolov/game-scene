#include "sdltexture.h"

sdltexture::sdltexture(TextureWrapper t, int w, int h) noexcept : _t(std::move(t)), _rect({ 0, 0, 0, 0 })
{
	_rect.w = w;
	_rect.h = h;
}

sdltexture::sdltexture(sdltexture&& other) noexcept : _t(std::move(other._t)), _rect(std::move(other._rect))
{
}

sdltexture& sdltexture::operator=(sdltexture&& other) noexcept
{
	if (this != &other) {
		_t = std::move(other._t);
		_rect = std::move(other._rect);
	}
	return *this;
}

void sdltexture::render(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, _t.get(), NULL, &_rect);
}

void sdltexture::setPos(int x, int y)
{
	_rect.x = x;
	_rect.y = y;
}

void sdltexture::setColorMod(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(_t.get(), r, g, b);
}
