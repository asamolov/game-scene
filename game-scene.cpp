// game-scene.cpp : Defines the entry point for the application.
//

#include "game-scene.h"
#include "sdlwindow.h"
#include <SDL.h>

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

        //The surface contained by the window
        SDL_Surface* screenSurface = NULL;

        //Get window surface
        screenSurface = window.GetWindowSurface();

        //Fill the surface white
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

        //Update the surface
        window.UpdateWindowSurface();

        //Hack to get window to stay up
        SDL_Event e;
        bool quit = false;
        while (quit == false) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT)
                    quit = true;
            }
        }
    }

    //Quit SDL subsystems
    SDL_Quit();
	return 0;
}
