#pragma once
#include "Game.h"

class Game;
class State
{
public:

	virtual void Clicked() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	Game* game;
	SDL_Renderer* renderer;
	State(SDL_Renderer* ren, Game* gm) { renderer = ren; game = gm; }
};