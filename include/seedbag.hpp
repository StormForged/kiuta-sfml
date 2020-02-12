#pragma once
#include <SFML/Graphics.hpp>

class SeedBag{
    public:
        SeedBag();
        SeedBag(int, int, sf::Texture&);
        sf::Sprite sprite;
        sf::IntRect colRect;
};