#pragma once
#include <SFML/Graphics.hpp>

class Bed{
    public:
        Bed();
        Bed(int, int, sf::Texture&);
        sf::Sprite sprite;
        sf::IntRect colRect;
};