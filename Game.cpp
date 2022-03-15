#include "Game.h"
#include "GameState.h"
#include "StartState.h"

void Game::Init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
			SDL_SetRenderDrawColor(renderer, 102, 194, 255, 255);

		isRunning = true;
	}
	if (TTF_Init() == 0)
		isRunning = true;
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		isRunning = true;
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		isRunning = true;

	currentState = new StartState(renderer, this);
}

void Game::HandleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		Quit();
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			currentState->Clicked();
		case SDL_BUTTON_RIGHT:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Game::Update()
{
	if (nextState != NULL)
	{
		currentState = nextState;

		nextState = NULL;
	}
	currentState->Update();
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 102, 194, 255, 255);
	currentState->Render();
	SDL_SetRenderDrawColor(renderer, 102, 194, 255, 255);
	SDL_RenderPresent(renderer);
}

void Game::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

void Game::ChangeState(State* state)
{
	nextState = state;
}