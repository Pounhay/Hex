#include "GameState.h"
#include "StartState.h"

void GameState::Clicked() {
	if (gameBoard->Over())
		game->ChangeState(new StartState(renderer, game));
	gameBoard->Clicked();
}

void GameState::Update() {
	gameBoard->Update();
}

void GameState::Render() {
	gameBoard->Render();
}