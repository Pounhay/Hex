#pragma once

#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_mixer.h>

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* texture, SDL_Renderer* ren)
	{
		SDL_Surface* tempSurface = IMG_Load(texture);
		SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
		SDL_FreeSurface(tempSurface);

		return tex;
	};
};