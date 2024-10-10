#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class label {
private:
	SDL_Rect _rect;
	TTF_Font* font;
public:
	label(TTF_Font* font);
	void setPos(int x, int y);
	void render(SDL_Renderer* renderer);
	std::string text;
};