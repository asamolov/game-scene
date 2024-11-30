/*
 * Collision system visualisation, using approach from Sedgewick's algorithm book/course.
 */

#include "CLI11.hpp"
#include "game-scene.h"
#include "collision-system.h"
#include <vector>
#include "particle.h"
#include "constants.h"
#include "label.h"
#include <filesystem>
#include <fmt/format.h>
#include <fmt/std.h>
#include <fmt/chrono.h>
#include <chrono>

int main(int argc, char* argv[])
{
    CLI::App cli{ "Collision System" };
    argv = cli.ensure_utf8(argv);

    bool vsync = true;
    cli.add_option("-v,--vsync", vsync, "enables or disables vsync")
        ->capture_default_str();

    unsigned int n = 100;
    auto n_option = cli.add_option("-n,--number", n, "number of random particles to generate")
        ->check(CLI::Range(1U, 10000U))
        ->capture_default_str();

    float radius = 0.01f;
    auto radius_option = cli.add_option("-r,--radius", radius, "radius of random particles, as fraction of screen size")
        ->check(CLI::Range(0.0f, 0.1f))
        ->capture_default_str();

    float speed_factor = 0.5f;
    auto speed_factor_option = cli.add_option("-s,--speed-factor", speed_factor, "max speed of random particles, as fraction of screen size")
        ->check(CLI::Range(0.0f, 1.0f))
        ->capture_default_str();

    std::filesystem::path filename;
    cli.add_option("-f,--file", filename, "file with particle system to load")
        ->excludes(n_option, radius_option, speed_factor_option)
        ->check(CLI::ExistingFile);

    CLI11_PARSE(cli, argc, argv);
    
    try {
        sdlapp app;
        sdlwindow window(app, "Collision System", SCREEN_WIDTH, SCREEN_HEIGHT, vsync);

        collision_system cs(window.LoadTexture("textures\\circle.png"));
        if (filename.empty()) {
            std::cout << fmt::format("Generating {} random particles of radius {} and max speed {}", n, radius, speed_factor) << std::endl;
            cs.generate(n, radius, speed_factor);
        } else {
            std::cout << fmt::format("Parsing particles from file {}...", filename) << std::endl;
            cs.load(filename);
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
            //std::cout << fmt::format("energy: {}", cs.energy()) << std::endl;
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
    for (size_t i = 0; i < particles.size(); i++) {
        auto& other = particles.at(i);
        auto time_to_hit = p.timeToHit(other);
        if (time_to_hit < prediction_limit) {
            pq.emplace(particles, OtherParticle, now + time_to_hit, idx, i);
        }
    }

    auto time_to_h_wall = p.timeToHitHorizontalWall();
    if (time_to_h_wall < prediction_limit) {
        pq.emplace(particles, HorizontalWall, now + time_to_h_wall, idx);
    }

    auto time_to_w_wall = p.timeToHitVerticalWall();
    if (time_to_w_wall < prediction_limit) {
        pq.emplace(particles, VerticalWall, now + time_to_w_wall, idx);
    }
}

collision_system::collision_system(sdltexture&& circle): _circle(std::move(circle)), now(0.f)
{
}

void collision_system::generate(const unsigned int n, const float radius, const float speed_factor)
{
    particle::setDefaultRadius(radius);
    particle::setSpeedFactor(speed_factor);
    particles.clear();
    particles.resize(n); // generates N particles using default ctor
    predict_all();
}

void collision_system::load(const std::filesystem::path& filename)
{
    std::ifstream infile(filename);

    int size;
    if (!(infile >> size)) {
        throw std::runtime_error(fmt::format("Unable to parse particles from file {}! Cause: {}", filename, "Unable to read number of particles in file"));
    }
    std::vector<particle> loaded_particles;
    loaded_particles.reserve(size);

    const double speed_factor = 20;
    double x, y;
    double vx, vy;
    double radius;
    double mass;
    unsigned short int r, g, b;
	while (infile >> x >> y >> vx >> vy >> radius >> mass >> r >> g >> b) {
        if (r > 255 || g > 255 || b > 255) {
            throw std::runtime_error(fmt::format("Unable to parse particles from file {}! Cause: incorrect rgb values: {} {} {}", filename, r, g, b));
        }
        loaded_particles.emplace_back(
            x, y, vx*speed_factor, vy*speed_factor, radius, mass, 
            static_cast<Uint8>(r), 
            static_cast<Uint8>(g),
            static_cast<Uint8>(b)
        );
    }
    if (loaded_particles.size() != size) {
        throw std::runtime_error(fmt::format("Unable to parse particles from file {}! Cause: got {}, expected {}", filename, loaded_particles.size(), size));
    }
    particles.swap(loaded_particles);
    predict_all();
}

void collision_system::predict_all()
{
    while (!pq.empty()) {
        pq.pop();
    }
    for (auto i = 0; i < particles.size(); i++) {
        predict(i);
    }
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

void collision_system::move(double dt)
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
        // discarding invalid events (already bounced)
        if (!e.is_valid()) {
            continue;
        }
        // moving all particles
        for (auto& p : particles) {
            p.move(time_to_event);
        }
        remaining -= time_to_event;
        now = e.time;

        // bouncing current one
        auto& p_a = particles.at(e.idx_a);
        switch (e.kind) {
        case HorizontalWall:
            p_a.bounceOffHorizontalWall();
            predict(e.idx_a);
            break;
        case VerticalWall:
            p_a.bounceOffVerticalWall();
            predict(e.idx_a);
            break;
        case OtherParticle:
            auto& p_b = particles.at(e.idx_b);
            p_a.bounceOff(p_b);
            predict(e.idx_a);
            predict(e.idx_b);
            break;
        }
    }

    if (now > next_compact) {
        compact();
        next_compact = now + compact_delta;
    }
}

void collision_system::render(SDL_Renderer* renderer)
{
    for (auto& p : particles) {
        p.render(renderer, _circle);
    }
}

double collision_system::energy() const
{
    double e = 0;
    for (auto& p : particles) {
        e += p.energy();
    }
    return e;
}

void collision_system::compact()
{
    auto start = std::chrono::high_resolution_clock::now();
    // std PQ implementation does not allow access to underlying container. 
    // Also, the heap function is private, so I cannot just extend and add compaction to that PQ.
    // requires custom PQ implementation to allow in-place compaction without allocating a new vector
    std::priority_queue<collision_event>::container_type tmp;
    auto old_size = pq.size();
    tmp.reserve(old_size);
    while (!pq.empty()) {
        auto& top = pq.top();
        if (top.is_valid()) {
            tmp.push_back(top);
        }
        pq.pop();
    }
    std::priority_queue<collision_event> new_pq(std::priority_queue<collision_event>::value_compare(), tmp);
    pq.swap(new_pq);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    std::cout << fmt::format("Compacted {}->{} in {}", old_size, pq.size(), std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)) << std::endl;
}