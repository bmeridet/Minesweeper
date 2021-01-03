#include <iostream>
#include <SFML/Graphics.hpp>
#include "Random.h"
#include "Board.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test Board");

    Board test;
    //test.RandomizeBoard();

    test.LoadFromFile("boards/testboard2.brd");

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
                    test.RevealBoard(click);
                }

                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    test.PlaceFlag(click);
                }
            }
        }

        window.clear();
        
        test.Draw(window);

        window.display();
    }

    TextureManager::Clear();

    return 0;
}