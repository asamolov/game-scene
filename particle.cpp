#include "particle.h"
#include "constants.h"
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
	move(seconds);
}

void particle::move(float seconds)
{
	x += vx * seconds;
	y += vy * seconds;
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

float particle::timeToHit(const particle& that) const
{
	return 0.0f;
}

float particle::timeToHitVerticalWall() const
{
	if (vx > 0) {
		return (1.0f - x - radius) / vx;
	}
	else {
		return (radius - x) / vx;
	}
}

float particle::timeToHitHorizontalWall() const
{
	if (vy > 0) {
		return (1.0f - y - radius) / vy;
	}
	else {
		return (radius - y) / vy;
	}
}

void particle::bounceOff(const particle& that)
{
}

void particle::bounceOffVerticalWall()
{
	vx = -vx;
}

void particle::bounceOffHorizontalWall()
{
	vy = -vy;
}

