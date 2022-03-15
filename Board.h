#pragma once

#include <ctime>
#include "Tile.h"
#include "Opponent.h"
#include "Text.h"

#define C 11
#define R 11

class Board
{
	SDL_Renderer* renderer;
	std::vector<std::vector<Tile*>> board;
	int mx, my, winner = 0;
	bool first;
	Opponent* opponent;
	std::vector<Tile*> up, left;
	Text* over;
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };
	SDL_Color red = { 255, 0, 0 };

public:

	Board(SDL_Renderer* ren, bool f, bool r) {
		board.resize(R, std::vector<Tile*>(C));
		renderer = ren;
		first = f;
		opponent = new Opponent(r);
		over = new Text(260, 250, 600, 200, renderer, white);
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				board[i][j] = new Tile(renderer, 77 + (j * 64) + (i * 32), 67 + (i * 56), r);
			}
		}
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (j < C - 1)
					board[i][j]->AddConnection(board[i][j + 1]);
				if (j > 0)
					board[i][j]->AddConnection(board[i][j - 1]);
				if (i < R - 1) {
					board[i][j]->AddConnection(board[i + 1][j]);
					if (j > 0)
						board[i][j]->AddConnection(board[i + 1][j - 1]);
				}
				if (i > 0) {
					board[i][j]->AddConnection(board[i - 1][j]);
					if (j < C - 1)
						board[i][j]->AddConnection(board[i - 1][j + 1]);
				}
				if (i == 0) {
					board[i][j]->SetUp();
					up.push_back(board[i][j]);
				}
				if (i == R - 1) {
					board[i][j]->SetDown();
				}
				if (j == 0) {
					board[i][j]->SetLeft();
					left.push_back(board[i][j]);
				}
				if (j == C - 1) {
					board[i][j]->SetRight();
				}
			}
		}
	}

	~Board();

	bool Over() { return winner != 0; }

	void Clicked() {
		SDL_GetMouseState(&mx, &my);
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				board[i][j]->IsClicked(mx, my);
			}
		}
		if(CheckWin() == 0)
			OpponentTurn();
	} 

	void OpponentTurn() {
		opponent->Choose(board, R, C);
	}

	int CheckWin()
	{
		std::vector<Tile*> possible;
		std::list<Tile*> queue;
		std::vector<Tile*> winners;


		for (int i = 0; i < up.size() - 1; i++) {
			if (up[i]->IsRed())
				winners.push_back(up[i]);
		}
		for (int i = 0; i < left.size() - 1; i++) {
			if (left[i]->IsBlack())
				winners.push_back(left[i]);
		}

		if (winners.empty())
			return 0;

		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				possible.push_back(board[i][j]);
			}
		}

		for (int i = 0; i < possible.size(); i++) {
			possible[i]->visited = false;
			possible[i]->distance = INT_MAX;
		}
		for (int i = 0; i < winners.size(); i++) {
			if (!winners[i]->visited) {
				int startID = std::distance(possible.begin(), std::find(possible.begin(), possible.end(), winners[i]));

				possible[startID]->distance = 0;
				queue.push_back(winners[i]);

				while (!queue.empty()) {
					Tile* aux = queue.front();
					int auxID = std::distance(possible.begin(), std::find(possible.begin(), possible.end(), aux));
					bool red = aux->IsRed();
					queue.pop_front();
					for (int i = 0; i < possible[auxID]->GetConnected().size(); i++) {
						if (possible[auxID]->GetConnected()[i]->visited == false && ((red && possible[auxID]->GetConnected()[i]->IsRed()) || (!red && possible[auxID]->GetConnected()[i]->IsBlack()))) {
							possible[auxID]->GetConnected()[i]->visited = true;
							queue.push_back(possible[auxID]->GetConnected()[i]);

							if (!red && possible[auxID]->GetConnected()[i]->IsRight())
								return 2;
							if (red && possible[auxID]->GetConnected()[i]->IsDown())
								return 1;
						}
					}
				}
			}
		}
		return 0;
	}

	void Update() {
		if (!first) {
			OpponentTurn();
			first = true;
		}
		winner = CheckWin();
		if (winner == 1) {
			over->Update("Red Wins");
			over->color = red;
		}
		if (winner == 2) {
			over->Update("Black Wins");
			over->color = black;
		}
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				board[i][j]->Update();
			}
		}
	}

	void Render() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 20, 67, 340, 627);
		SDL_RenderDrawLine(renderer, 769, 67, 1089, 627);
		SDL_RenderDrawLine(renderer, 21, 67, 341, 627);
		SDL_RenderDrawLine(renderer, 770, 67, 1090, 627);
		SDL_RenderDrawLine(renderer, 22, 67, 342, 627);
		SDL_RenderDrawLine(renderer, 771, 67, 1091, 627);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 77, 4, 717, 4);
		SDL_RenderDrawLine(renderer, 397, 674, 1037, 674);
		SDL_RenderDrawLine(renderer, 77, 5, 717, 5);
		SDL_RenderDrawLine(renderer, 397, 675, 1037, 675);
		SDL_RenderDrawLine(renderer, 77, 6, 717, 6);
		SDL_RenderDrawLine(renderer, 397, 676, 1037, 676);

		SDL_GetMouseState(&mx, &my);
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				board[i][j]->Render(mx, my);
			}
		}
		if (winner != 0)
			over->Render();
	}
};
