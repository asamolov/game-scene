#include <SDL.h>

class rect {
private:
	SDL_Rect _rect;

public:
	rect(int x, int y, int w, int h);
	rect(SDL_Rect& other);
	void render(SDL_Renderer* renderer);
};