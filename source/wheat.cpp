#include "../include/wheat.hpp"

Wheat::Wheat(){}

Wheat::Wheat(int x, int y, sf::Texture& texture){
    sprite.setOrigin(0, 32);
    sprite.setTexture(texture);
    sprite.setPosition(x * 32, y * 32);
}