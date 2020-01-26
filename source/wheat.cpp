#include "../include/wheat.hpp"

Wheat::Wheat(){}

Wheat::Wheat(int x, int y, std::vector<sf::Texture>& textures){
    sprite.setOrigin(0, 32);
    sprite.setTexture(textures[0]);
    sprite.setPosition(x * 32, y * 32);
}

void Wheat::update(std::vector<sf::Texture>& textures){
    days_planted++;
    if(state < harvestable){
        state++;
        sprite.setTexture(textures[state]);
    }
}