#include "Application.h"

void Application::Initialize()
{
	mineSweeper.RandomizeBoard();

	reset.setTexture(TextureManager::GetTexture("face_happy"));
	debug.setTexture(TextureManager::GetTexture("debug"));
	testOne.setTexture(TextureManager::GetTexture("test_1"));
	testTwo.setTexture(TextureManager::GetTexture("test_2"));
	testThree.setTexture(TextureManager::GetTexture("test_3"));

    pos.x = 0.0;
    pos.y = 0.0;
}

void Application::Update()
{
    window.create(sf::VideoMode(800, 600), "Minesweeper");

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // if click... check tiles and switch textures...
            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f click(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // If click in board rectangle...
                    mineSweeper.RevealBoard(click);
                    // Else check UI buttons...
                }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    mineSweeper.PlaceFlag(click);
                }
            }
        }

        window.clear();

        //DrawBoard(pos);
        //DrawUI(pos);
        mineSweeper.Draw(window);

        window.display();
    }
}

void Application::Shutdown()
{
    TextureManager::Clear();
}

void Application::DrawBoard(sf::Vector2f& pos)
{
    Tile* board = mineSweeper.GetBoard();

    sf::Vector2i tileDimension(board->GetSprite().getTextureRect().width, board->GetSprite().getTextureRect().height);

    Tile* end = board + (mineSweeper.GetWidth() * mineSweeper.GetHeight());
    int count = 0;
    for (Tile* i = board; i != end; ++i)
    {
        i->SetTexture();
        sf::Sprite& temp = i->GetSprite();
        temp.setPosition(pos);

        window.draw(temp);
        std::cout << "Drew tile " << count << std::endl;
        
        if (i->MineState() && !i->HiddenState())
        {
            sf::Sprite mine(TextureManager::GetTexture("mine"));
            mine.setPosition(pos);
            window.draw(mine);
        }

        if (i->FlagState() && i->HiddenState())
        {
            sf::Sprite flag(TextureManager::GetTexture("flag"));
            flag.setPosition(pos);
            window.draw(flag);
        }

        if (!i->HiddenState() && !i->MineState())
        {
            int numMines = i->AdjacentMines();

            if (numMines > 0)
            {
                sf::Sprite number(TextureManager::GetTexture(GetTileNumber(numMines)));
                number.setPosition(pos);
                window.draw(number);
            }
        }

        pos.x += tileDimension.x;

        if (count % mineSweeper.GetHeight() == 0)
        {
            pos.x = 0;
            pos.y += tileDimension.y;
        }
        
        count += 1;
    }

    pos.x = (tileDimension.x * mineSweeper.GetHeight()) / 2.0f;
}

void Application::DrawUI(sf::Vector2f& pos)
{
    // Draw digits...

    sf::Sprite digits(TextureManager::GetTexture("digits"));
    digits.setPosition(sf::Vector2f(0, pos.y));
    window.draw(digits);

    /*===============================================================================*/

    reset.setOrigin(sf::Vector2f((reset.getTextureRect().width / 2.0f), 0));
    reset.setPosition(sf::Vector2f(pos.x, pos.y));
    window.draw(reset);

    pos.x += reset.getTextureRect().width * 2.0f;

    debug.setOrigin(sf::Vector2f((reset.getTextureRect().width / 2.0f), 0));
    debug.setPosition(sf::Vector2f(pos.x, pos.y));
    window.draw(debug);

    pos.x += reset.getTextureRect().width;

    testOne.setOrigin(sf::Vector2f((reset.getTextureRect().width / 2.0f), 0));
    testOne.setPosition(sf::Vector2f(pos.x, pos.y));
    window.draw(testOne);

    pos.x += reset.getTextureRect().width;

    testTwo.setOrigin(sf::Vector2f((reset.getTextureRect().width / 2.0f), 0));
    testTwo.setPosition(sf::Vector2f(pos.x, pos.y));
    window.draw(testTwo);

    pos.x += reset.getTextureRect().width;

    testThree.setOrigin(sf::Vector2f((reset.getTextureRect().width / 2.0f), 0));
    testThree.setPosition(sf::Vector2f(pos.x, pos.y));
    window.draw(testThree);
}

const char* Application::GetTileNumber(int x)
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