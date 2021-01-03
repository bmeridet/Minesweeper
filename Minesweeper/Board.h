#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include "Random.h"
#include "TextureManager.h"
#include "Tile.h"
#include "UIButton.h"

class Board
{
public:
	Board();
	~Board();

	int GetMineCount();

	void RandomizeBoard();
	void LoadFromFile(const char* fileName);

	void RevealBoard(sf::Vector2f& click);
	void RevealMines();
	void FlagMines();

	void PlaceFlag(sf::Vector2f& click);

	void toggleDebug();

	void Draw(sf::RenderWindow& window);
	void Reset();

	const int GetWidth();
	const int GetHeight();

	bool GetGameStatus();
	bool CheckIfWin();
	bool CheckIfLose(Tile& tile);

	Tile* GetBoard();

private:
	void FillAdjacentTiles();
	void GetAdjacent(int x, int y);

	const char* GetTileNumber(int x);

	void RecurseTiles(Tile& tile);

	void DrawOptions(sf::RenderWindow& window, sf::Vector2f& pos);
	void DrawCount(sf::RenderWindow& window, sf::Vector2f& pos);
	void DrawDigit(sf::RenderWindow& window, sf::Vector2f& digitPlacement, sf::IntRect& rect, sf::Sprite& digits, int width, int x);

	static const int WIDTH = 16;
	static const int HEIGHT = 25;

	Tile* board[WIDTH][HEIGHT];
	std::map<std::string, UIButton> uiButtons;

	sf::FloatRect boardRect;
	sf::FloatRect uiRect;

	int mineCount = 0;

	bool debug = false;
	bool gameOver = false;
};

