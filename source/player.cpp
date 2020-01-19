#include "../include/player.hpp"

Player::Player(){}

Player::Player(int x, int y, sf::Texture& texture){
    sprite.setTexture(texture);
    sprite.setPosition(x,y);
    direction = SOUTH;
}