#include <SDL.h>
#include <string>

class sdlwindow
{
private:
	SDL_Window* window = nullptr;
public:
	sdlwindow(const std::string& title, int width, int height);
	SDL_Surface* GetWindowSurface();
	void UpdateWindowSurface();
	~sdlwindow();
};
