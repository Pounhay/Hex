#pragma once

#include "TextureManager.h"

class Text
{
	SDL_Rect destRect;
	SDL_Renderer* renderer;
	SDL_Texture* message;

public:
	int xpos, ypos;
	int h1, w1;
	SDL_Color color;

	Text() {}
	~Text() {};

	Text(int x, int y, int w, int h, SDL_Renderer* ren, SDL_Color col) {
		xpos = x;
		ypos = y;
		h1 = h;
		w1 = w;
		renderer = ren;
		color = col;
	}

	void Update(const char* s)
	{
		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = w1;
		destRect.h = h1;
		TTF_Font* Chalk = TTF_OpenFont("./Assets/ChalkDash-O2j4.ttf", 64);
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Chalk, s, color);
		message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	}

	void Render() {
		SDL_RenderCopy(renderer, message, NULL, &destRect);
	}
};