#pragma once

#include "Board.h"
#include <vector>
#include "State.h"

class State;
class Game
{
	bool isRunning = false;
	State* currentState;
	State* nextState;

	SDL_Window* window;
	SDL_Renderer* renderer;

public:
	Game() {}
	~Game() {}

	bool Running() { return isRunning; }
	void Quit() { isRunning = false; }

	void Init(const char* title, int width, int height, bool fullscreen);
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
	void ChangeState(State* state);
};
