#pragma once
#include <SFML/Graphics.hpp>

class Wheat{
    public:
        sf::Sprite sprite;
        int days_planted = 0;
        int state = 0;
        Wheat(sf::Texture&);   
};