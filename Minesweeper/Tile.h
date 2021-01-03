#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "TextureManager.h"

class Tile
{
public:
	Tile();

	void SetTexture();
	sf::Sprite& GetSprite();

	bool FlagState();
	bool MineState();
	bool HiddenState();

	void RevealTile();
	void SetFlag();
	void SetMine();
	void AddAdjacentTile(Tile* t);

	int AdjacentMines();
	std::vector<Tile*>& GetAdjTiles();

	void Reset();

private:
	sf::Sprite sprite;
	bool isHidden = true;
	bool hasFlag = false;
	bool isMine = false;
	std::vector<Tile*> adjacentTiles;
};

