/*
 * Collision system visualisation, using approach from Sedgewick's algorithm book/course.
 */

#include "game-scene.h"
#include "collision-system.h"
#include <vector>
#include "particle.h"

int main(int argc, char* argv[])
{
    try {
        sdlapp app;
        sdlwindow window(app, "Collision System", SCREEN_WIDTH, SCREEN_HEIGHT);

        auto circle = window.LoadTexture("textures\\circle.png");
        const int n = 1000;

        std::vector<particle> particles;
        particles.reserve(n);
        for (auto i = 0; i < n; i++) {
            particles.push_back(particle());
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
            auto now = SDL_GetTicks();
            auto dt = now - last_frame;
            last_frame = now;
            for (auto& p : particles) {
                p.move(dt);
            }
            auto renderer = window.BeginRendering();
            for (auto& p : particles) {
                p.render(renderer, circle);
            }
            window.EndRendering();
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
	return 0;
}
