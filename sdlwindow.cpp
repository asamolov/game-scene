#include "sdlwindow.h"

#include <SDL_ttf.h>
#include <stdexcept>
#include <fmt/core.h>

sdlwindow::sdlwindow(const sdlapp& _app, const std::string& title, int width, int height) : 
	app(_app), font(_app.loadFont(fps_font_path, fps_font_size)), counter(font, 0, 0)
{
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		throw std::runtime_error(fmt::format("Window could not be created! SDL_Error: {}", SDL_GetError()));
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // | SDL_RENDERER_PRESENTVSYNC
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
	counter.Render(renderer);
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
	// TODO: Switch to smart wrappers
	if (font != nullptr) {
		TTF_CloseFont(font);
		font = nullptr;
	}
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
	if (window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}

fpscounter::fpscounter(TTF_Font* font, int x, int y) : label(font), time(SDL_GetTicks()), frames(0)
{
	label.setPos(x, y);
}

void fpscounter::Render(SDL_Renderer* renderer)
{
	if (frames % fps_frames_avg == 0) {
		auto now = SDL_GetTicks();
		auto fps = frames * 1000.f / (now - time);
		if (fps > 2000000) {
			fps = 0;
		}
		auto fpsLabel = fmt::format("FPS: {:3.2f}", fps);
		label.text = fpsLabel;
		Reset();
	}
	label.render(renderer);
	frames++;
}

void fpscounter::Reset()
{
	time = SDL_GetTicks();
	frames = 0;
}