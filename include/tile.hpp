#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Tile{
    public:
        Tile();
        Tile(int, int, sf::Texture&);
        int type;
        bool passable;
        bool crop;
        sf::Sprite sprite;
        sf::IntRect tileRect;

    enum type{
        terrain,        
        farmable,
        buildable,
        busy
    };
};