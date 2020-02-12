#include "../include/bed.hpp"

Bed::Bed(){}

Bed::Bed(int x, int y, sf::Texture& texture){
    sprite.setTexture(texture);
    sprite.setPosition(x, y);

    colRect.left = x;
    colRect.top = y;
    colRect.height = 32;
    colRect.width = 32;
}