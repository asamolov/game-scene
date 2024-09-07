// game-scene.cpp : Defines the entry point for the application.
//

#include "game-scene.h"
#include "sdlwindow.h"
#include "geometry.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    {
        //The window we'll be rendering to
        sdlwindow window("SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT);

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
            window.EndRendering();
        }
    }

    //Quit SDL subsystems
    SDL_Quit();
	return 0;
}
