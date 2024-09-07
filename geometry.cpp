#include "geometry.h"

rect::rect(int x, int y, int w, int h)
{
    _rect = { x, y, w, h };
}

rect::rect(SDL_Rect& other)
{
    _rect = other;
}

void rect::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &_rect);
}
