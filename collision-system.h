#pragma once

#include <vector>
#include <queue>
#include "particle.h"
#include "sdltexture.h"

enum collision_kind {
	HorizontalWall, VerticalWall, OtherParticle
};

struct collision_event {
	float time;
	collision_kind kind;
	size_t idx_a, idx_b;
	collision_event(collision_kind kind, float time, size_t idx_a) : time(time), kind(kind), idx_a(idx_a), idx_b(idx_a) {

	}

	friend bool operator<(const collision_event& lhs, const collision_event& rhs) {
		return lhs.time > rhs.time; // reverse comparison, so priority_queue returns event with smallest time on top
	}
};


class collision_system {
private:
	std::priority_queue<collision_event> pq;
	std::vector<particle> particles;
	sdltexture _circle;
	float now = 0.f;
	void predict(size_t idx);
public:
	collision_system(sdltexture&& circle);
	void add_particle(particle&& p);
	void move(Uint32 dt);
	void move(float dt);
	void render(SDL_Renderer* renderer);
};