#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <set>
#include "Board.h"
#include "Random.h"

Board::Board()
{
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			Tile* t = new Tile;
			board[i][j] = t;
		}
	}

	FillAdjacentTiles();

	uiButtons["control"].setTexture(TextureManager::GetTexture("face_happy"));
	uiButtons["debug"].setTexture(TextureManager::GetTexture("debug"));
	uiButtons["testOne"].setTexture(TextureManager::GetTexture("test_1"));
	uiButtons["testTwo"].setTexture(TextureManager::GetTexture("test_2"));
	uiButtons["testThree"].setTexture(TextureManager::GetTexture("test_3"));

	boardRect.width = board[0][0]->GetSprite().getGlobalBounds().width * HEIGHT;
	boardRect.height = board[0][0]->GetSprite().getGlobalBounds().width * WIDTH;

	uiRect.top = boardRect.height;
	uiRect.width = boardRect.width;
	uiRect.height = uiButtons["control"].getGlobalBounds().height;
}


Board::~Board()
{
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			delete board[i][j];
		}
	}
}

int Board::GetMineCount() { return mineCount; }

void Board::RandomizeBoard()
{
	Reset();

	std::set<int> randNumbers;
	while (randNumbers.size() < 50)
		randNumbers.insert(Random::Int(0, 399));

	std::set<int>::iterator it;
	for (it = randNumbers.begin(); it != randNumbers.end(); ++it)
	{
		int i = *it / HEIGHT;
		int j = *it % HEIGHT;
		board[i][j]->SetMine();
		mineCount += 1;
	}
}

void Board::LoadFromFile(const char* fileName)
{
	Reset();

	std::ifstream inFS(fileName);

	int count = 0;
	char cNum;
	while (!inFS.eof())
	{
		inFS.get(cNum);

		if (cNum == '1')
		{
			int i = count / HEIGHT;
			int j = count % HEIGHT;
			board[i][j]->SetMine();
			mineCount += 1;
		}
		if (cNum != '\n')
			count += 1;

	}
}

void Board::RevealBoard(sf::Vector2f& click)
{
	if (boardRect.contains(click) && !gameOver)
	{
		int count = 0;
		for (int i = 0; i < WIDTH; ++i)
		{
			for (int j = 0; j < HEIGHT; ++j)
			{
				Tile& tile = *board[i][j];
				sf::Sprite& temp = board[i][j]->GetSprite();

				if (temp.getGlobalBounds().contains(click) && !tile.FlagState())
				{
					tile.RevealTile();
					if (!CheckIfLose(tile))
					{
						RecurseTiles(tile);
						CheckIfWin();
					}
				}
			}
		}
	}
	else if (uiRect.contains(click))
	{

		if (uiButtons["control"].getGlobalBounds().contains(click))
		{
			Reset();
			RandomizeBoard();
		}
		else if (uiButtons["debug"].getGlobalBounds().contains(click))
		{
			toggleDebug();
		}
		else if (uiButtons["testOne"].getGlobalBounds().contains(click))
		{
			LoadFromFile("boards/testboard1.brd");
		}
		else if (uiButtons["testTwo"].getGlobalBounds().contains(click))
		{
			LoadFromFile("boards/testboard2.brd");
		}
		else if (uiButtons["testThree"].getGlobalBounds().contains(click))
		{
			LoadFromFile("boards/testboard3.brd");
		}
	}
}

void Board::RevealMines()
{
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			if (board[i][j]->MineState())
				board[i][j]->RevealTile();
		}
	}
}

void Board::FlagMines()
{
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			if (board[i][j]->MineState() && !board[i][j]->FlagState())
			{
				board[i][j]->SetFlag();
				mineCount -= 1;
			}
		}
	}
}

void Board::PlaceFlag(sf::Vector2f& click)
{
	if (!gameOver)
	{
		for (int i = 0; i < WIDTH; ++i)
		{
			for (int j = 0; j < HEIGHT; ++j)
			{
				Tile& tile = *board[i][j];
				sf::Sprite& temp = board[i][j]->GetSprite();

				if (temp.getGlobalBounds().contains(click))
				{
					tile.SetFlag();
					if (tile.FlagState())
					{
						mineCount -= 1;
					}
					else
					{
						mineCount += 1;
					}
				}
			}
		}
	}
}

void Board::toggleDebug()
{
	if (!debug)
	{
		debug = true;
	}
	else
	{
		debug = false;
	}
}

void Board::Draw(sf::RenderWindow& window)
{
	sf::Vector2f origin = board[0][0]->GetSprite().getOrigin();

	sf::Vector2i tileDimension(board[0][0]->GetSprite().getTextureRect().width, board[0][0]->GetSprite().getTextureRect().height);

	sf::Vector2f pos(origin.x, origin.y);
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			board[i][j]->SetTexture();
			sf::Sprite& temp = board[i][j]->GetSprite();
			temp.setPosition(pos);

			window.draw(temp);

			if (board[i][j]->MineState() && !board[i][j]->HiddenState())
			{
				sf::Sprite mine(TextureManager::GetTexture("mine"));
				mine.setPosition(pos);
				window.draw(mine);
			}

			if (board[i][j]->FlagState() && board[i][j]->HiddenState())
			{
				sf::Sprite flag(TextureManager::GetTexture("flag"));
				flag.setPosition(pos);
				window.draw(flag);
			}

			if (!board[i][j]->HiddenState() && !board[i][j]->MineState())
			{
				int numMines = board[i][j]->AdjacentMines();

				if (numMines > 0)
				{
					sf::Sprite number(TextureManager::GetTexture(GetTileNumber(numMines)));
					number.setPosition(pos);
					window.draw(number);
				}
			}

			if (debug && board[i][j]->MineState())
			{
				sf::Sprite mine(TextureManager::GetTexture("mine"));
				mine.setPosition(pos);
				window.draw(mine);
			}

			pos.x += tileDimension.x;
		}

		pos.x = origin.x;
		pos.y += tileDimension.y;
	}

	pos.x = (tileDimension.x * HEIGHT) / 2.0f;
	DrawOptions(window, pos);
}

void Board::Reset()
{
	mineCount = 0;

	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			board[i][j]->Reset();
		}
	}

	uiButtons["control"].setTexture(TextureManager::GetTexture("face_happy"));
	gameOver = false;
}

const int Board::GetWidth() { return WIDTH; }
const int Board::GetHeight() { return HEIGHT; }

bool Board::GetGameStatus() { return gameOver; }

bool Board::CheckIfWin()
{
	// win - All tiles that are not mines are revealed...
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			if (board[i][j]->HiddenState() && !board[i][j]->MineState())
				return false;
		}
	}

	FlagMines();
	uiButtons["control"].setTexture(TextureManager::GetTexture("face_win"));
	gameOver = true;
	return true;
}
bool Board::CheckIfLose(Tile& tile)
{
	if (tile.MineState())
	{
		RevealMines();
		uiButtons["control"].setTexture(TextureManager::GetTexture("face_lose"));
		gameOver = true;
		return true;
	}
	return false;
}

Tile* Board::GetBoard() { return board[0][0]; }

/*====================================== Private Helper Functions ==========================================*/

void Board::GetAdjacent(int x, int y)
{
	for (int i = (x > 0 ? -1 : 0); i <= (x < (WIDTH - 1) ? 1 : 0); ++i)
	{
		for (int j = (y > 0 ? -1 : 0); j <= (y < (HEIGHT - 1) ? 1 : 0); ++j)
		{
			if (i != 0 || j != 0)
				board[x][y]->AddAdjacentTile(board[x + i][y + j]);
		}
	}
}

void Board::FillAdjacentTiles()
{
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			GetAdjacent(i, j);
		}
	}
}

const char* Board::GetTileNumber(int x)
{
	switch (x)
	{
	case 1:
		return "number_1";
	case 2:
		return "number_2";
	case 3:
		return "number_3";
	case 4:
		return "number_4";
	case 5:
		return "number_5";
	case 6:
		return "number_6";
	case 7:
		return "number_7";
	case 8:
		return "number_8";
	default:
		return "Error";
	}
}

void Board::RecurseTiles(Tile& tile)
{
	int numMines = tile.AdjacentMines();

	if (numMines == 0)
	{

		std::vector<Tile*>& tiles = tile.GetAdjTiles();

		for (unsigned int i = 0; i < tiles.size(); ++i)
		{

			if (tiles.at(i)->HiddenState() && !tiles.at(i)->MineState() && !tiles.at(i)->FlagState())
			{
				tiles.at(i)->RevealTile();
				RecurseTiles(*tiles.at(i));
			}
		}
	}
}

void Board::DrawOptions(sf::RenderWindow& window, sf::Vector2f& pos)
{
	// Draw digits...

	DrawCount(window, pos);
	
	// Make a container to loop through this code...

	std::map<std::string, UIButton>::iterator it;
	for (it = uiButtons.begin(); it != uiButtons.end(); ++it)
	{
		if (it->first == "control")
		{
			it->second.setOrigin(sf::Vector2f((it->second.getTextureRect().width / 2.0f), 0));
			it->second.setPosition(sf::Vector2f(pos.x, pos.y));
			window.draw(it->second);

			pos.x += it->second.getTextureRect().width * 2.0f;
		}
		else
		{
			it->second.setOrigin(sf::Vector2f((it->second.getTextureRect().width / 2.0f), 0));
			it->second.setPosition(sf::Vector2f(pos.x, pos.y));
			window.draw(it->second);

			pos.x += it->second.getTextureRect().width;
		}
	}
}

void Board::DrawCount(sf::RenderWindow& window, sf::Vector2f& pos)
{
	std::vector<int> numbers;
	int number = abs(mineCount);
	while (number > 0)
	{
		int digit = number % 10;
		numbers.push_back(digit);
		number /= 10;
	}

	sf::Vector2f digitPlacement(0, pos.y);

	sf::Sprite digits(TextureManager::GetTexture("digits"));

	int numWidth = (TextureManager::GetTexture("digits").getSize().x) / 11;
	int numHeight = (TextureManager::GetTexture("digits").getSize().y);

	sf::IntRect rect(0, 0, numWidth, numHeight);
	
	if (mineCount < 0)
	{
		DrawDigit(window, digitPlacement, rect, digits, numWidth, 10);
	}

	for (int i = 1; i <= (3 - numbers.size()); ++i)
	{
		DrawDigit(window, digitPlacement, rect, digits, numWidth, 0);
	}
	
	for (int i = numbers.size() - 1; i >= 0; --i)
	{
		switch (numbers.at(i))
		{
		case 0:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 0);
			break;
		}
		case 1:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 1);
			break;
		}
		case 2:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 2);
			break;
		}
		case 3:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 3);
			break;
		}
		case 4:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 4);
			break;
		}
		case 5:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 5);
			break;
		}
		case 6:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 6);
			break;
		}
		case 7:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 7);
			break;
		}
		case 8:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 8);
			break;
		}
		case 9:
		{
			DrawDigit(window, digitPlacement, rect, digits, numWidth, 9);
			break;
		}
		default:
			break;
		}
	}
}

void Board::DrawDigit(sf::RenderWindow& window, sf::Vector2f& digitPlacement, sf::IntRect& rect, sf::Sprite& digits, int width, int x)
{
	rect.left = width * x;
	digits.setTextureRect(rect);
	digits.setPosition(digitPlacement);
	window.draw(digits);
	digitPlacement.x += width;
	rect.left = 0;
}