#pragma once

#include "State.h"

class GameState : public State
{
	Board* gameBoard;

public:
	GameState(SDL_Renderer* ren, Game* gm, bool f, bool r) : State(ren, gm) {
		gameBoard = new Board(renderer, f, r);
	}

	void Clicked();
	void Update();
	void Render();
};
