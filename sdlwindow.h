#include <SDL.h>
#include <string>
#include "label.h"

constexpr int fps_frames_avg = 10;

class fpscounter {
private:
	label label;
	Uint32 time;
	Uint32 frames;
public:
	fpscounter(TTF_Font* font, int x, int y);
	void Reset();
	void Render(SDL_Renderer* renderer);
};

class sdlwindow
{
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
public:
	sdlwindow(const std::string& title, int width, int height);
	SDL_Surface* GetWindowSurface();
	SDL_Renderer* BeginRendering();
	void EndRendering();
	void UpdateWindowSurface();
	~sdlwindow();
};
