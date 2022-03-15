#include "StartState.h"
#include "GameState.h"

void StartState::Update() {
	Red->Update("Red");
	Black->Update("Black");
	First->Update("First");
	Second->Update("Second");
	IsOver();
}

void StartState::Render() {
	if (!menu) {
		Red->Render();
		Black->Render();
	}
	else {
		First->Render();
		Second->Render();
	}
}

void StartState::IsOver() {
	int mx1, my1;
	SDL_GetMouseState(&mx1, &my1);
	if (menu) {
		if ((mx1 > First->xpos) && (mx1 < First->xpos + First->w1) && (my1 > First->ypos) && (my1 < First->ypos + First->h1))
			First->color = yellow;
		else if ((mx1 > Second->xpos) && (mx1 < Second->xpos + Second->w1) && (my1 > Second->ypos) && (my1 < Second->ypos + Second->h1))
			Second->color = yellow;
		else {
			Second->color = white;
			First->color = white;
		}
	}
	else {
		if ((mx1 > Red->xpos) && (mx1 < Red->xpos + Red->w1) && (my1 > Red->ypos) && (my1 < Red->ypos + Red->h1))
			Red->color = yellow;
		else if ((mx1 > Black->xpos) && (mx1 < Black->xpos + Black->w1) && (my1 > Black->ypos) && (my1 < Black->ypos + Black->h1))
			Black->color = yellow;
		else {
			Black->color = white;
			Red->color = white;
		}
	}
}

void StartState::Clicked() {
	int mx1, my1;
	SDL_GetMouseState(&mx1, &my1);
	if (menu) {
		if ((mx1 > First->xpos) && (mx1 < First->xpos + First->w1) && (my1 > First->ypos) && (my1 < First->ypos + First->h1)) {
			first = true;
			menu = false;
		}
		else if ((mx1 > Second->xpos) && (mx1 < Second->xpos + Second->w1) && (my1 > Second->ypos) && (my1 < Second->ypos + Second->h1)) {
			first = false;
			menu = false;
		}
		else {
			Second->color = white;
			First->color = white;
		}
	}
	else {
		if ((mx1 > Red->xpos) && (mx1 < Red->xpos + Red->w1) && (my1 > Red->ypos) && (my1 < Red->ypos + Red->h1)) {
			red = true;
			game->ChangeState(new GameState(renderer, game, first, red));
		}
		else if ((mx1 > Black->xpos) && (mx1 < Black->xpos + Black->w1) && (my1 > Black->ypos) && (my1 < Black->ypos + Black->h1)) {
			red = false;
			game->ChangeState(new GameState(renderer, game, first, red));
		}
		else {
			Black->color = white;
			Red->color = white;
		}
	}
}