#pragma once

#include <SDL.h>
#include "sdltexture.h"

class particle {
private:
	double x, y;
	double vx, vy;
	double radius;
	double mass;
	size_t _collisions;
	SDL_Color color;
	SDL_Color color_of_speed() const;
public:
	particle();
	particle(double x, double y, double vx, double vy, double radius, double mass, Uint8 r, Uint8 g, Uint8 b);
	void move(Uint32 dt);
	void move(double seconds);
	void render(SDL_Renderer* renderer, sdltexture& texture);
	double timeToHit(const particle& that) const;
	double timeToHitVerticalWall() const;
	double timeToHitHorizontalWall() const;
	void bounceOff(particle& that);
	void bounceOffVerticalWall();
	void bounceOffHorizontalWall();
	size_t collisions() const;
	double energy() const;
};