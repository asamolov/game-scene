// game-scene.cpp : Defines the entry point for the application.
//

#include "game-scene.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
    try {
        sdlapp app;
        sdlwindow window(app, "SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT);
        auto t = window.LoadTexture("textures\\circle.png");
        t.setPos(200, 200);

        rect r = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

        // Main event loop
        SDL_Event e;
        bool quit = false;
        while (quit == false) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }
            auto renderer = window.BeginRendering();
            r.render(renderer);
            t.render(renderer);
            window.EndRendering();
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
	return 0;
}
