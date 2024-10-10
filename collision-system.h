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
	std::reference_wrapper<const std::vector<particle>> particles;
	size_t idx_a, idx_b;
	size_t collisions_a, collisions_b;
	collision_event(const std::vector<particle>& particles, collision_kind kind, float time, size_t idx_a) : 
		particles(std::cref(particles)),
		time(time), 
		kind(kind), 
		idx_a(idx_a), 
		idx_b(idx_a) {
		collisions_a = particles.at(idx_a).collisions();
		collisions_b = particles.at(idx_b).collisions();
	}

	friend bool operator<(const collision_event& lhs, const collision_event& rhs) {
		return lhs.time > rhs.time; // reverse comparison, so priority_queue returns event with smallest time on top
	}

	bool is_valid() const {
		return collisions_a == particles.get().at(idx_a).collisions()
			&& collisions_b == particles.get().at(idx_b).collisions();
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