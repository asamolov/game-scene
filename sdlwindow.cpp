#include "sdlwindow.h"

#include <stdexcept>
#include <fmt/core.h>

sdlwindow::sdlwindow(const std::string& title, int width, int height)
{
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		throw std::runtime_error(fmt::format("Window could not be created! SDL_Error: {}", SDL_GetError()));
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		throw std::runtime_error(fmt::format("Renderer for window {} could not be created! SDL_Error: {}", fmt::ptr(window), SDL_GetError()));
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

SDL_Surface* sdlwindow::GetWindowSurface()
{
	auto surface = SDL_GetWindowSurface(window);
	if (surface == nullptr) {
		throw std::runtime_error(fmt::format("cannot get surface from window {}! SDL_Error: {}", fmt::ptr(window), SDL_GetError()));
	}
	return surface;
}

SDL_Renderer* sdlwindow::BeginRendering()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	return renderer;
}

void sdlwindow::EndRendering()
{
	SDL_RenderPresent(renderer);
}

void sdlwindow::UpdateWindowSurface()
{
	if (SDL_UpdateWindowSurface(window) != 0) {
		throw std::runtime_error(fmt::format("Failed to update surface for window {}! SDL_Error: {}", fmt::ptr(window), SDL_GetError()));
	}
}

sdlwindow::~sdlwindow()
{
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	if (window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}