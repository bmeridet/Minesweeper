#include <iostream>
#include "Tile.h"

Tile::Tile()
{
	sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
}

void Tile::SetTexture()
{
	if (!isHidden)
		sprite.setTexture(TextureManager::GetTexture("tile_revealed"));
	else
		sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
}

sf::Sprite& Tile::GetSprite()
{
	return sprite;
}

bool Tile::FlagState()
{
	return hasFlag;
}

bool Tile::MineState()
{
	return isMine;
}

bool Tile::HiddenState()
{
	return isHidden;
}

void Tile::RevealTile()
{
	if (isHidden)
	{
		isHidden = false;
	}
}

void Tile::SetFlag()
{
	if (!hasFlag)
	{
		hasFlag = true;
	}
	else
	{
		hasFlag = false;
	}
}

void Tile::SetMine()
{
	isMine = true;
}

void Tile::AddAdjacentTile(Tile* t)
{
	adjacentTiles.push_back(t);
}

int Tile::AdjacentMines()
{
	int count = 0;

	for (int i = 0; i < adjacentTiles.size(); ++i)
	{
		if (adjacentTiles.at(i)->MineState())
			count += 1;
	}

	return count;
}

std::vector<Tile*>& Tile::GetAdjTiles()
{
	return adjacentTiles;
}

void Tile::Reset()
{
	isHidden = true;
	hasFlag = false;
	isMine = false;

	SetTexture();

	//adjacentTiles.clear();
}