#pragma once
#include <SFML/Graphics.hpp>

class Wheat{
    public:
        Wheat();
        Wheat(int, int, std::vector<sf::Texture>&);
        void update(std::vector<sf::Texture>&);

        std::vector<sf::Texture> textures;
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