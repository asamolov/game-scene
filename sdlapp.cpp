#include "sdlapp.h"

#include <SDL_ttf.h>
#include <stdexcept>
#include <fmt/core.h>

sdlapp::sdlapp() {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error(fmt::format("SDL could not initialize! SDL_Error: {}", SDL_GetError()));
    }
    //Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        throw std::runtime_error(fmt::format("SDL_ttf could not initialize! SDL_ttf Error: {}", TTF_GetError()));
    }
}

TTF_Font* sdlapp::loadFont(const char* path, int fontSize) const
{
    // TODO: add lookup of ttf file using this code: https://stackoverflow.com/questions/11387564/get-a-font-filepath-from-name-and-style-in-c-windows
    // C:\Windows\Fonts\arial.ttf
    auto font = TTF_OpenFont(path, fontSize);
    if (font == nullptr)
    {
        throw std::runtime_error(fmt::format("Failed to load font {}! TTF_Error: {}", path, TTF_GetError()));
    }
    return font;
}

sdlapp::~sdlapp()
{
    //Quit SDL subsystems
    TTF_Quit();
    SDL_Quit();
}
