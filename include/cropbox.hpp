#pragma once
#include <SFML/Graphics.hpp>
#include "collidable.hpp"

class CropBox : public Collidable{
    public:
        CropBox(int, int, sf::Texture&);
        int crops;
        int crop_history;
        int current_crops;
};