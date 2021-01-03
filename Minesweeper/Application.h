#pragma once

#include <iostream>
#include <vector>
#include "Board.h"

class Application
{
public:
	void Initialize();
	void Update();
	void Shutdown();

	void DrawBoard(sf::Vector2f& pos);
	void DrawUI(sf::Vector2f& pos);

	const char* GetTileNumber(int x);

private:
	sf::Vector2f pos;
	
	UIButton reset;
	UIButton debug;
	UIButton testOne;
	UIButton testTwo;
	UIButton testThree;

	Board mineSweeper;

	sf::RenderWindow window;
};

