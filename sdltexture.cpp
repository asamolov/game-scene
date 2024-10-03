#include "sdltexture.h"

sdltexture::sdltexture(TextureWrapper t, int w, int h) : _t(std::move(t)), _rect({ 0, 0, 0, 0 })
{	
	_rect.w = w;
	_rect.h = h;
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