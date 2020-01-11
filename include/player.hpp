#pragma once
#include <SFML/Graphics.hpp>

class Player{
    public:
        Player(int, int, sf::Texture&);
        sf::Sprite sprite;
        sf::Vector2f velocity;
};