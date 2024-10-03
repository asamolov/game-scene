#pragma once

#include <SDL_surface.h>
#include <memory>

struct SurfaceDeleter {
	void operator()(SDL_Surface* p) const {
		SDL_FreeSurface(p);
	}
};
using SurfaceWrapper = std::unique_ptr<SDL_Surface, SurfaceDeleter>;


struct TextureDeleter {
	void operator()(SDL_Texture* p) const {
		SDL_DestroyTexture(p);
	}
};
using TextureWrapper = std::unique_ptr<SDL_Texture, TextureDeleter>;
