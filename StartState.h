#pragma once

#include "State.h"
#include "Text.h"

class StartState : public State
{
private:
	Text* First;
	Text* Second;
	Text* Red;
	Text* Black;
	SDL_Color white = { 255, 255, 255 };
	SDL_Color yellow = { 255, 255, 102 };
	bool menu = true;
	bool first = true;
	bool red = true;

public:
	void Update();
	void Render();
	void IsOver();
	void Clicked();

	StartState(SDL_Renderer* ren, Game* gm) : State(ren, gm) {
		Red = new Text(360, 230, 400, 64, renderer, white);
		Black = new Text(360, 460, 400, 64, renderer, white);
		First = new Text(360, 230, 400, 64, renderer, white);
		Second = new Text(360, 460, 400, 64, renderer, white);
	}
};

