#pragma once

#include "TextureManager.h"
#include <cmath>
#include <vector>

class Tile
{
	int xpos, ypos;
	int h1 = 74;
	int w1 = 64;
	int type = 0;
	bool red = true;
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool goal = false;

	SDL_Texture* objNone;
	SDL_Texture* objOpponent;
	SDL_Texture* objAvailable;
	SDL_Texture* objPlayer;
	SDL_Rect scrRect, destRect;
	SDL_Renderer* renderer;
	std::vector<Tile*> connected;

public:
	bool visited = false;
	Tile* before = NULL;
	int distance = INT_MAX;

	Tile();
	~Tile();

	Tile(SDL_Renderer* ren, int x, int y, bool r) {
		renderer = ren;
		if (r) {
			objOpponent = TextureManager::LoadTexture("./Assets/Black.png", renderer);
			objPlayer = TextureManager::LoadTexture("./Assets/Red.png", renderer);
		}
		else {
			objOpponent = TextureManager::LoadTexture("./Assets/Red.png", renderer);
			objPlayer = TextureManager::LoadTexture("./Assets/Black.png", renderer);
		}
		objNone = TextureManager::LoadTexture("./Assets/Tile.png", renderer);
		objAvailable = TextureManager::LoadTexture("./Assets/Available.png", renderer);
		xpos = x;
		ypos = y;
		red = r;
	}

	bool IsAvailable() { return type == 0 ? true : false; }
	bool IsPlayer() { return type == 1 ? true : false; }
	bool IsOpponent() { return type == 2 ? true : false; }
	bool IsRed() { return (type == 1 && red == true) || (type == 2 && red == false) ? true : false; }
	bool IsBlack() { return (type == 2 && red == true) || (type == 1 && red == false) ? true : false; }
	void SetUp() { up = true; }
	void SetDown() { down = true; }
	void SetLeft() { left = true; }
	void SetRight() { right = true; }
	bool IsRight() { return right; }
	bool IsLeft() { return left; }
	bool IsUp() { return up; }
	bool IsDown() { return down; }
	bool IsGoal() { return goal; }
	void SetGoal() { goal = true; }
	void UnSetGoal() { goal = true; }


	std::vector<Tile*> GetConnected() { return connected; }

	void Update() {
		scrRect.w = w1;
		scrRect.h = h1;
		scrRect.x = 0;
		scrRect.y = 0;

		destRect.x = xpos - w1/2;
		destRect.y = ypos - h1/2;
		destRect.w = scrRect.w;
		destRect.h = scrRect.h;
	}
	
	void Render(int mx, int my) {
		switch (type) {
			case 0:
				if(IsHovering(mx, my))
					SDL_RenderCopy(renderer, objAvailable, &scrRect, &destRect);
				else
					SDL_RenderCopy(renderer, objNone, &scrRect, &destRect);
				break;
			case 1:
				SDL_RenderCopy(renderer, objPlayer, &scrRect, &destRect);
				break;
			case 2:
				SDL_RenderCopy(renderer, objOpponent, &scrRect, &destRect);
				break;
		}
	}

	void Opponent() { type = 2; }
	
	void AddConnection(Tile* toConnect) {
		connected.push_back(toConnect);
	}

	bool IsHovering(int mx, int my) {
		int dx = mx - xpos;
		int dy = my - ypos;
		float distance = std::sqrt(dx * dx + dy * dy);
		if (distance < w1 / 2)
			return true;
		else
			return false;
	}

	void IsClicked(int mx, int my) {
		if (IsHovering(mx, my) && type == 0)
			type = 1;
	}
};