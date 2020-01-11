#include "../include/player.hpp"

Player::Player(int x, int y, sf::Texture& texture){
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    velocity = sf::Vector2f(0, 0);
}