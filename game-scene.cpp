﻿// game-scene.cpp : Defines the entry point for the application.
//

#include "game-scene.h"
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
    try {
        sdlapp app;
        sdlwindow window(app, "SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT);

        std::vector<sdltexture> textures;
        auto red = window.LoadTexture("textures\\circle.png");
        red.setPos(100, 0);
        red.setColorMod(255, 0, 0);
        auto green = window.LoadTexture("textures\\circle.png");
        green.setPos(200, 0);
        green.setColorMod(0, 255, 0);
        auto blue = window.LoadTexture("textures\\circle.png");
        blue.setPos(300, 0);
        blue.setColorMod(0, 0, 255);
        textures.push_back(std::move(red));
        textures.push_back(std::move(green));
        textures.push_back(std::move(blue));
        //textures.emplace_back(std::move(red), std::move(green), std::move(blue));

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
            for (auto& t : textures) {
                t.render(renderer);
            }
            window.EndRendering();
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
	return 0;
}
