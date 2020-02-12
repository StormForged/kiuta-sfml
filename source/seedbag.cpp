#include "../include/seedbag.hpp"

SeedBag::SeedBag(){}

SeedBag::SeedBag(int x, int y, sf::Texture& texture){
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    
    colRect.left = x;
    colRect.top = y;
    colRect.height = 32;
    colRect.width = 32;
}
