#include "particle.h"
#include "collision-system.h"
#include <cstdlib>

// random float between 0 and 1
float randf()
{
	return std::rand() / static_cast<float>(RAND_MAX);
}

particle::particle()
{
	x = randf();
	y = randf();
	vx = randf() / 10.f - 0.05f;
	vy = randf() / 10.f - 0.05f;
	radius = 0.01f;
	mass = 1.f;
	color = { 0,0,0,0 };
}

void particle::move(Uint32 dt)
{
	auto seconds = dt / 1000.f;
	auto dx = vx * seconds;
	auto dy = vy * seconds;
	if ((x + dx < radius) || (x + dx > 1.f - radius)) { vx = -vx; dx = -dx; }
	if ((y + dy < radius) || (y + dy > 1.f - radius)) { vy = -vy; dy = -dy; }
	x += dx;
	y += dy;
}

void particle::render(SDL_Renderer* renderer, sdltexture& texture)
{
	texture.setColorMod(color);
	SDL_FRect rect = {
		(x - radius) * SCREEN_WIDTH,
		(y - radius) * SCREEN_HEIGHT,
		radius * 2 * SCREEN_WIDTH,
		radius * 2 * SCREEN_WIDTH
	};
	texture.render(renderer, rect);
}

