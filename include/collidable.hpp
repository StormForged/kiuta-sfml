#pragma once
#include <SFML/Graphics.hpp>

class Collidable{
    public:
        virtual ~Collidable(){}
        Collidable(int, int, sf::Texture&);
        sf::Sprite sprite;
        sf::IntRect colRect;
};