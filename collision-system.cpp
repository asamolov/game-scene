/*
 * Collision system visualisation, using approach from Sedgewick's algorithm book/course.
 */

#include "game-scene.h"
#include "collision-system.h"
#include <vector>
#include "particle.h"
#include "constants.h"

int main(int argc, char* argv[])
{
    try {
        sdlapp app;
        sdlwindow window(app, "Collision System", SCREEN_WIDTH, SCREEN_HEIGHT);

        const int n = 1000;

        collision_system cs(window.LoadTexture("textures\\circle.png"));
        for (auto i = 0; i < n; i++) {
            cs.add_particle(particle());
        }

        // Main event loop
        SDL_Event e;
        bool quit = false;
        auto last_frame = SDL_GetTicks();
        while (quit == false) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            /// ---- move this out to a separate class? 
            auto now = SDL_GetTicks();
            auto dt = now - last_frame;
            last_frame = now;
            cs.move(dt);
            auto renderer = window.BeginRendering();
            cs.render(renderer);
            ///
            window.EndRendering();
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
	return 0;
}

void collision_system::predict(size_t idx)
{
    auto& p = particles.at(idx);
    pq.emplace(HorizontalWall, now + p.timeToHitHorizontalWall(), idx);
    pq.emplace(VerticalWall, now + p.timeToHitVerticalWall(), idx);
}

collision_system::collision_system(sdltexture&& circle): _circle(std::move(circle)), now(0.f)
{
}

void collision_system::add_particle(particle&& p)
{
    particles.push_back(std::move(p));
    predict(particles.size() - 1);
}

void collision_system::move(Uint32 dt)
{
    move(dt / 1000.f);
}

void collision_system::move(float dt)
{
    auto remaining = dt;
    while (!pq.empty()) {
        auto e = pq.top();
        auto time_to_event = e.time - now;
        if (time_to_event > remaining) {
            // all past events are handled, stopping for now
            // moving all particles
            for (auto& p : particles) {
                p.move(remaining);
            }
            now += remaining;
            break;
        }
        pq.pop();
        // moving all particles
        for (auto& p : particles) {
            p.move(time_to_event);
        }
        remaining -= time_to_event;
        now = e.time;

        // bouncing current one
        auto& p = particles.at(e.idx_a);
        switch (e.kind) {
        case HorizontalWall:
            p.bounceOffHorizontalWall();
            pq.emplace(HorizontalWall, now + p.timeToHitHorizontalWall(), e.idx_a);
            break;
        case VerticalWall:
            p.bounceOffVerticalWall();
            pq.emplace(VerticalWall, now + p.timeToHitVerticalWall(), e.idx_a);
            break;
        case OtherParticle:
            break;
        }
    }
}

void collision_system::render(SDL_Renderer* renderer)
{
    for (auto& p : particles) {
        p.render(renderer, _circle);
    }
}
