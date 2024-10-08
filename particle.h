#pragma once

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
	void move(float seconds);
	void render(SDL_Renderer* renderer, sdltexture& texture);
	float timeToHit(const particle& that) const;
	float timeToHitVerticalWall() const;
	float timeToHitHorizontalWall() const;
	void bounceOff(const particle& that);
	void bounceOffVerticalWall();
	void bounceOffHorizontalWall();
};