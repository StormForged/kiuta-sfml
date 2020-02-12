#include "../include/cropbox.hpp"

CropBox::CropBox(int x, int y, sf::Texture& texture) : Collidable(x, y, texture){
    crops = 0;
    crop_history = 0;
    current_crops = 0;
}