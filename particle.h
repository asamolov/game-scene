#include <SDL.h>
#include "sdltexture.h"

class particle {
private:
	float x, y;
	float vx, vy;
	float radius;
	float mass;
	SDL_Color color;
public:
	particle();
	void move(Uint32 dt);
	void render(SDL_Renderer* renderer, sdltexture& texture);
};