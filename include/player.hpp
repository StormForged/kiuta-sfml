#pragma once
#include <SFML/Graphics.hpp>

class Player{
    public:
        Player();
        Player(int, int, sf::Texture&);
        sf::Sprite sprite;
        int direction;

        enum direction{
            NORTH,
            EAST,
            SOUTH,
            WEST
        };
};