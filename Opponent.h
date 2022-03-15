#pragma once

#include <vector>
#include <list>
#include "Tile.h"

class Opponent
{
	bool black;

public:

	Opponent(bool bl) { black = bl; }

	Opponent() {}
	~Opponent() {}

	void Choose(std::vector<std::vector<Tile*>> board, int R, int C) {
		std::vector<Tile*> availableTiles;
		std::vector<Tile*> possibleTiles;
		std::vector<Tile*> playerTiles;
		std::vector<Tile*> opponentTiles;
		std::vector<Tile*> winTilesOpponent0;
		std::vector<Tile*> winTilesPlayer0;
		std::vector<Tile*> winTilesOpponent1;
		std::vector<Tile*> winTilesPlayer1;
		int type = 0, typePlayer = 0;
		bool moved = false;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				possibleTiles.push_back(board[i][j]);
				if (board[i][j]->IsAvailable())
					availableTiles.push_back(board[i][j]);
				if (board[i][j]->IsPlayer()) {
					playerTiles.push_back(board[i][j]);
					if ((!black && board[i][j]->IsRight()) || (black && board[i][j]->IsDown())) {
						if (typePlayer == 2 || typePlayer == 3)
							typePlayer = 3;
						else
							typePlayer = 1;
						winTilesPlayer0.push_back(board[i][j]);
					}
					if ((!black && board[i][j]->IsLeft()) || (black && board[i][j]->IsUp())) {
						winTilesPlayer1.push_back(board[i][j]);
						if (typePlayer == 1 || typePlayer == 3)
							typePlayer = 3;
						else
							typePlayer = 2;
					}
				}
				if (board[i][j]->IsOpponent()) {
					moved = true;
					opponentTiles.push_back(board[i][j]);
					if ((black && board[i][j]->IsRight()) || (!black && board[i][j]->IsDown())) {
						if (type == 2 || type == 3)
							type = 3;
						else
							type = 1;
						winTilesOpponent0.push_back(board[i][j]);
					}
					if ((black && board[i][j]->IsLeft()) || (!black && board[i][j]->IsUp())) {
						winTilesOpponent1.push_back(board[i][j]);
						if (type == 1 || type == 3)
							type  = 3;
						else
							type = 2;
					}
				}
			}
		}
		if (!moved) {
			int i = (availableTiles.size() - 1) / 2;
			availableTiles[i]->Opponent();
		}
		else {
			selection(possibleTiles, opponentTiles, playerTiles, winTilesPlayer0, winTilesPlayer1, winTilesOpponent0, winTilesOpponent1, type, typePlayer)->Opponent();
		}
	}

private:

	Tile* selection(std::vector<Tile*> possible, std::vector<Tile*> opponent, std::vector<Tile*> player, std::vector<Tile*> playerWinTiles0, std::vector<Tile*> playerwinTiles1, std::vector<Tile*> opponentWinTiles0, std::vector<Tile*> opponentWinTiles1, int type, int typePlayer) {
		int distance = INT_MAX;
		Tile* choose = NULL;
		Tile* aux = NULL;
		std::vector<Tile*> opponentPath, playerPath;
		if (type != 3) {
			for (int i = 0; i < opponent.size(); i++) {
				int index = std::distance(possible.begin(), std::find(possible.begin(), possible.end(), opponent[i]));
				aux = BFS(possible, opponent[i], index, type, false, typePlayer);
				if (aux != NULL && aux->distance < distance) {
					distance = aux->distance;
					opponentPath.clear();
					while (aux->before != opponent[i]) {
						opponentPath.push_back(aux);
						aux = aux->before;
					}
					opponentPath.push_back(aux);
				}
			}
			choose = opponentPath.back();
		}

		distance = INT_MAX;
		if (ConnectedCount(player) > 3) {
			if (type == 3)
				opponentPath = Connected(possible, opponent, opponentWinTiles0, opponentWinTiles1, true);
			if (typePlayer != 3) {
				for (int i = 0; i < player.size(); i++) {
					int index = std::distance(possible.begin(), std::find(possible.begin(), possible.end(), player[i]));
					aux = BFS(possible, player[i], index, type, true, typePlayer);
					if (aux != NULL && aux->distance < distance) {
						distance = aux->distance;
						playerPath.clear();
						while (aux->before != player[i]) {
							playerPath.push_back(aux);
							aux = aux->before;
						}
						playerPath.push_back(aux);
					}
				}
			}
			else 
				playerPath = Connected(possible, player, playerWinTiles0, playerwinTiles1, false);

			bool intersect = false;
			for (int i = 0; i < playerPath.size(); i++) {
				auto it = std::find(opponentPath.begin(), opponentPath.end(), playerPath[i]);
				if (it != opponentPath.end()) {
					choose = *it;
					intersect = true;
					break;
				}
			}
			if (!intersect) {
				if (playerPath.size() < opponentPath.size() - 2|| opponentPath.empty()) {
					choose = playerPath.back();
				}
				else {
					choose = opponentPath.back();
				}
			}
		}

		return choose;
	}
	

	Tile* BFS(std::vector<Tile*> possible, Tile* start, int startID, int target, bool player, int targetPlayer)
	{
		std::list<Tile*> queue;

		for (int i = 0; i < possible.size(); i++) {
			possible[i]->visited = false;
			possible[i]->distance = INT_MAX;
		}

		possible[startID]->distance = 0;
		queue.push_back(start);

		while (!queue.empty()) {
			Tile* aux = queue.front();
			int auxID = std::distance(possible.begin(), std::find(possible.begin(), possible.end(), aux));
			queue.pop_front();
			for (int i = 0; i < possible[auxID]->GetConnected().size(); i++) {
				if (possible[auxID]->GetConnected()[i]->visited == false && possible[auxID]->GetConnected()[i]->IsAvailable()) {
					possible[auxID]->GetConnected()[i]->visited = true;
					possible[auxID]->GetConnected()[i]->distance = possible[auxID]->distance + 1;
					possible[auxID]->GetConnected()[i]->before = aux;
					queue.push_back(possible[auxID]->GetConnected()[i]);

					if (!player) {
						switch (target) {
						case 0:
							if ((black && (possible[auxID]->GetConnected()[i]->IsLeft() || possible[auxID]->GetConnected()[i]->IsRight())) || (!black && (possible[auxID]->GetConnected()[i]->IsDown() || possible[auxID]->GetConnected()[i]->IsUp())))
								return possible[auxID]->GetConnected()[i];
							break;
						case 1:
							if ((black && possible[auxID]->GetConnected()[i]->IsLeft()) || (!black && possible[auxID]->GetConnected()[i]->IsUp()))
								return possible[auxID]->GetConnected()[i];
							break;
						case 2:
							if ((black && possible[auxID]->GetConnected()[i]->IsRight()) || (!black && possible[auxID]->GetConnected()[i]->IsDown()))
								return possible[auxID]->GetConnected()[i];
							break;
						}
					}
					else {
						switch (targetPlayer) {
						case 0:
							if ((!black && (possible[auxID]->GetConnected()[i]->IsLeft() || possible[auxID]->GetConnected()[i]->IsRight())) || (black && (possible[auxID]->GetConnected()[i]->IsDown() || possible[auxID]->GetConnected()[i]->IsUp())))
								return possible[auxID]->GetConnected()[i];
							break;
						case 1:
							if ((!black && possible[auxID]->GetConnected()[i]->IsLeft()) || (black && possible[auxID]->GetConnected()[i]->IsUp()))
								return possible[auxID]->GetConnected()[i];
							break;
						case 2:
							if ((!black && possible[auxID]->GetConnected()[i]->IsRight()) || (black && possible[auxID]->GetConnected()[i]->IsDown()))
								return possible[auxID]->GetConnected()[i];
							break;
						}
					}
				}
			}
		}
		return NULL;
	}

	int ConnectedCount(std::vector<Tile*> player)
	{
		if (player.empty())
			return 0;

		Tile* connected = NULL;
		std::list<Tile*> queue;
		int distance = -1;

		for (int i = 0; i < player.size(); i++) {
			player[i]->visited = false;
			player[i]->distance = INT_MAX;
		}

		(*player.begin())->distance = 0;
		queue.push_back(*player.begin());

		while (!queue.empty()) {
			Tile* aux = queue.front();
			int auxID = std::distance(player.begin(), std::find(player.begin(), player.end(), aux));
			queue.pop_front();
			for (int i = 0; i < player[auxID]->GetConnected().size(); i++) {
				if (player[auxID]->GetConnected()[i]->visited == false && player[auxID]->GetConnected()[i]->IsPlayer()) {
					player[auxID]->GetConnected()[i]->visited = true;
					queue.push_back(player[auxID]->GetConnected()[i]);
					player[auxID]->GetConnected()[i]->distance = player[auxID]->distance + 1;
					if (distance < player[auxID]->GetConnected()[i]->distance) {
						connected = player[auxID]->GetConnected()[i];
						distance = connected->distance;
					}
				}
			}
		}
		return distance;;
	}

	std::vector<Tile*> Connected(std::vector<Tile*> possible, std::vector<Tile*> used, std::vector<Tile*> winTiles0, std::vector<Tile*> winTiles1, bool op)
	{

		std::vector<Tile*> connected;
		std::vector<Tile*> path;
		std::list<Tile*> queue;
		Tile* temp = NULL;
		
		int distance = -1;

		for (int i = 0; i < used.size(); i++) 
			used[i]->visited = false;

		for (int i = 0; i < winTiles0.size(); i++) {
			if (!winTiles0[i]->visited) {
				connected.push_back(winTiles0[i]);
				winTiles0[i]->distance = 0;
				queue.push_back(winTiles0[i]);

				while (!queue.empty()) {
					Tile* aux = queue.front();
					int auxID = std::distance(used.begin(), std::find(used.begin(), used.end(), aux));
					queue.pop_front();
					for (int i = 0; i < used[auxID]->GetConnected().size(); i++) {
						if (used[auxID]->GetConnected()[i]->visited == false && ((op && used[auxID]->GetConnected()[i]->IsOpponent()) || (!op && used[auxID]->GetConnected()[i]->IsPlayer()))) {
							used[auxID]->GetConnected()[i]->visited = true;
							queue.push_back(used[auxID]->GetConnected()[i]);
							connected.push_back(used[auxID]->GetConnected()[i]);
						}
					}
				}
			}
		}

		for (int i = 0; i < winTiles1.size(); i++) {
			if (!winTiles1[i]->visited) {
				winTiles1[i]->SetGoal();
				winTiles1[i]->distance = 0;
				queue.push_back(winTiles1[i]);

				while (!queue.empty()) {
					Tile* aux = queue.front();
					int auxID = std::distance(used.begin(), std::find(used.begin(), used.end(), aux));
					queue.pop_front();
					for (int i = 0; i < used[auxID]->GetConnected().size(); i++) {
						if (used[auxID]->GetConnected()[i]->visited == false && ((op && used[auxID]->GetConnected()[i]->IsOpponent()) || (!op && used[auxID]->GetConnected()[i]->IsPlayer()))) {
							used[auxID]->GetConnected()[i]->visited = true;
							queue.push_back(used[auxID]->GetConnected()[i]);
							used[auxID]->GetConnected()[i]->SetGoal();
						}
					}
				}
			}
		}

		for (int i = 0; i < connected.size(); i++) {
			for (int i = 0; i < possible.size(); i++) {
				possible[i]->visited = false;
				possible[i]->distance = INT_MAX;
			}

			int index = std::distance(possible.begin(), std::find(possible.begin(), possible.end(), connected[i]));
			possible[index]->distance = 0;
			queue.push_back(connected[i]);

			while (!queue.empty()) {
				Tile* aux = queue.front();
				int auxID = std::distance(possible.begin(), std::find(possible.begin(), possible.end(), aux));
				queue.pop_front();
				for (int i = 0; i < possible[auxID]->GetConnected().size(); i++) {
					if (possible[auxID]->GetConnected()[i]->visited == false && possible[auxID]->GetConnected()[i]->IsAvailable()) {
						possible[auxID]->GetConnected()[i]->visited = true;
						possible[auxID]->GetConnected()[i]->distance = possible[auxID]->distance + 1;
						possible[auxID]->GetConnected()[i]->before = aux;
						queue.push_back(possible[auxID]->GetConnected()[i]);

						if (possible[auxID]->GetConnected()[i]->IsGoal())
							temp = possible[auxID]->GetConnected()[i];

					}
				}
			}
			if (temp != NULL && temp->distance < distance) {
				distance = temp->distance;
				path.clear();
				while (temp->before != connected[i]) {
					path.push_back(temp);
					temp = temp->before;
				}
				path.push_back(temp);
			}
		}

		return path;
	}
};