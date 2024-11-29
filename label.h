#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "sdlwrappers.h"

class label {
private:
	SDL_Rect _rect;
	TTF_Font* font;
	TextureWrapper _texture;
	std::string _text;
public:
	label(TTF_Font* font);
	void setPos(int x, int y);
	void render(SDL_Renderer* renderer);
	void set_text(const std::string& text);
};