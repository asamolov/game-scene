#include "label.h"
#include "sdlwrappers.h"

#include <stdexcept>
#include <fmt/core.h>
#include <iostream>

label::label(TTF_Font* font): font(font), _rect({0, 0, 0, 0}), _texture()
{
}

void label::setPos(int x, int y)
{
	_rect.x = x;
	_rect.y = y;
}

void label::render(SDL_Renderer* renderer)
{
	if (_text.empty()) {
		// not rendering empty strings
		return;
	}
	if (!_texture) {
		SurfaceWrapper textSurface(TTF_RenderText_Blended(font, _text.c_str(), SDL_Color{ 0x00, 0x00, 0x00, 0x00 }));
		TextureWrapper texture(SDL_CreateTextureFromSurface(renderer, textSurface.get()));
		_rect.h = textSurface->h;
		_rect.w = textSurface->w;
		_texture = std::move(texture);
		//std::cout << fmt::format("New texture {}x{}", _rect.w, _rect.h) << std::endl;
	}
	SDL_RenderCopy(renderer, _texture.get(), NULL, &_rect);
}

void label::set_text(const std::string& text)
{
	if (text != _text) {
		_text = text;
		_texture.reset();
		//std::cout << fmt::format("New text {}", _text) << std::endl;
	}
}
