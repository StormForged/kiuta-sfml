#pragma once
#include <SFML/Graphics.hpp>

class Wheat{
    public:
        Wheat();
        Wheat(int, int, sf::Texture&);

        sf::Sprite sprite;
        int days_planted = 0;
        int state = seed;

    enum state{
        seed,
        sprout, 
        growing,
        harvestable
    };
};