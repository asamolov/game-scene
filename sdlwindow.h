#include <SDL.h>
#include <string>

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
