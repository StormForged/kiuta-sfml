#include "../include/tile.hpp"

Tile::Tile(){}

Tile::Tile(int x, int y, sf::Texture& texture){
    sprite.setPosition(x, y);
    sprite.setTexture(texture);
    type = terrain;
    passable = true;
    crop = false;

    tileRect.left = x;
    tileRect.top = y;
    tileRect.height = 32;
    tileRect.width = 32;
}