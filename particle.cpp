#include "particle.h"
#include "constants.h"
#include <cstdlib>
#include <cmath>

// random double between 0 and 1
double randf()
{
	return std::rand() / static_cast<double>(RAND_MAX);
}

const double speed_factor = 0.3f;

particle::particle()
{
	x = randf();
	y = randf();
	vx = (randf() - 0.5f) * speed_factor;
	vy = (randf() - 0.5f) * speed_factor;
	radius = 0.01f;
	mass = 1.f;
	color = { 0,0,0,0 };
	_collisions = 0;
}

void particle::move(Uint32 dt)
{
	auto seconds = dt / 1000.f;
	move(seconds);
}

void particle::move(double seconds)
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

double particle::timeToHit(const particle& that) const
{
	if (this == &that) return INFINITY;
	double dx = that.x - x;
	double dy = that.y - y;
	double dvx = that.vx - vx;
	double dvy = that.vy - vy;
	double dvdr = dx * dvx + dy * dvy;
	if (dvdr > 0) return INFINITY;
	double dvdv = dvx * dvx + dvy * dvy;
	if (dvdv == 0) return INFINITY;
	double drdr = dx * dx + dy * dy;
	double sigma = radius + that.radius;
	double d = (dvdr * dvdr) - dvdv * (drdr - sigma * sigma);
	if (d < 0) return INFINITY;
	double t = -(dvdr + std::sqrt(d)) / dvdv;

	// should't happen, but seems to be needed for some extreme inputs
	// (doubleing-point precision when dvdv is close to 0, I think)
	if (t <= 0) return INFINITY;

	return t;
}

double particle::timeToHitVerticalWall() const
{
	if (vx > 0) {
		return (1.0f - x - radius) / vx;
	}
	else {
		return (radius - x) / vx;
	}
}

double particle::timeToHitHorizontalWall() const
{
	if (vy > 0) {
		return (1.0f - y - radius) / vy;
	}
	else {
		return (radius - y) / vy;
	}
}

void particle::bounceOff(particle& that)
{
	double dx = that.x - x;
	double dy = that.y - y;
	double dvx = that.vx - vx;
	double dvy = that.vy - vy;
	double dvdr = dx * dvx + dy * dvy;             // dv dot dr
	double dist = radius + that.radius;   // distance between particle centers at collison

	// magnitude of normal force
	double magnitude = 2 * mass * that.mass * dvdr / ((mass + that.mass) * dist);

	// normal force, and in x and y directions
	double fx = magnitude * dx / dist;
	double fy = magnitude * dy / dist;

	// update velocities according to normal force
	vx += fx / mass;
	vy += fy / mass;
	that.vx -= fx / that.mass;
	that.vy -= fy / that.mass;

	if (std::abs(vx) > 1 || std::abs(vy) > 1 || std::abs(that.vx) > 1 || std::abs(that.vy) > 1) {
		__debugbreak();
	}

	// update collision counts
	_collisions++;
	that._collisions++;
}

void particle::bounceOffVerticalWall()
{
	vx = -vx;
	_collisions++;
}

void particle::bounceOffHorizontalWall()
{
	vy = -vy;
	_collisions++;
}

size_t particle::collisions() const
{
	return _collisions;
}

double particle::energy() const
{
	return 0.5 * mass * (vx * vx + vy * vy);
}

