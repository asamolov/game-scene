#include "particle.h"
#include "constants.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

// random double between 0 and 1
double randf()
{
	return std::rand() / static_cast<double>(RAND_MAX);
}

const double speed_factor = 0.5f;

particle::particle()
{
	x = randf();
	y = randf();
	vx = (randf() - 0.5f) * speed_factor;
	vy = (randf() - 0.5f) * speed_factor;
	radius = 0.01f;
	mass = 1.f;
	color = { 0,0,0,255 };
	_collisions = 0;
}

particle::particle(double x, double y, double vx, double vy, double radius, double mass, Uint8 r, Uint8 g, Uint8 b):
	x(x), y(y), vx(vx), vy(vy), radius(radius), mass(mass), color({r, g, b, 0}), _collisions(0)
{
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
	// if alpha is 255 - use color of speed
	if (color.a == 255) {
		texture.setColorMod(color_of_speed());
	} else {
		texture.setColorMod(color);
	}
	SDL_FRect rect = {
		(x - radius) * SCREEN_WIDTH,
		(y - radius) * SCREEN_HEIGHT,
		radius * 2 * SCREEN_WIDTH,
		radius * 2 * SCREEN_WIDTH
	};
	texture.render(renderer, rect);
}

const SDL_Color minC = { 0, 0, 255, 0 };
const double minV = 0;
const SDL_Color maxC = { 255, 0, 0, 0 };
const double maxV = speed_factor;

SDL_Color particle::color_of_speed() const {
	double ratio = (std::clamp(std::sqrt(vx * vx + vy * vy), minV, maxV) - minV) / (maxV - minV);
	return {
		static_cast<Uint8> (minC.r * (1 - ratio) + maxC.r * ratio),
		static_cast<Uint8> (minC.g * (1 - ratio) + maxC.g * ratio),
		static_cast<Uint8> (minC.b * (1 - ratio) + maxC.b * ratio),
		static_cast<Uint8> (minC.a * (1 - ratio) + maxC.a * ratio)
	};
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
	} else if (vx < 0) {
		return (radius - x) / vx;
	} else {
		return INFINITY;
	}
}

double particle::timeToHitHorizontalWall() const
{
	if (vy > 0) {
		return (1.0f - y - radius) / vy;
	} else if (vy < 0) {
		return (radius - y) / vy;
	} else {
		return INFINITY;
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

