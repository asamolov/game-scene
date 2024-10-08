#include <SDL.h>
#include <string>
#include "label.h"
#include "sdlapp.h"
#include "sdltexture.h"

constexpr int fps_frames_avg = 30;
constexpr int fps_font_size = 24;
constexpr const char* fps_font_path = "C:\\Windows\\Fonts\\arial.ttf";

class fpscounter {
private:
	label label;
	Uint32 time;
	Uint32 frames;
	void Reset();
public:
	fpscounter(TTF_Font* font, int x, int y);
	void Render(SDL_Renderer* renderer);
};

class sdlwindow
{
private:
	const sdlapp& app;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr; // TODO: move fonts management to app
	fpscounter counter;
public:
	sdlwindow(const sdlapp& _app, const std::string& title, int width, int height, bool vsync = true);
	sdltexture LoadTexture(const std::string& path);
	SDL_Surface* GetWindowSurface();
	SDL_Renderer* BeginRendering();
	void EndRendering();
	void UpdateWindowSurface();
	~sdlwindow();
};
