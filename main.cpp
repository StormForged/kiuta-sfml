#include <sstream>
#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "include/tile.hpp"
#include "include/player.hpp"
#include "include/wheat.hpp"

template<typename T>
std::string toString(T arg){
    std::stringstream ss;
    ss << arg;
    return ss.str();
}

int main(){
    //Window init
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Kisetsu no Uta - Prototype");
    window.setFramerateLimit(60);
    sf::Event event;
    sf::Color transparentColor(0,0,0,0);

    sf::Vector2i mapSize(1280, 736);
    sf::Vector2i mapSize_grid(mapSize.x / 32, mapSize.y / 32);
    sf::Vector2i viewPosition(mapSize.x / 2, mapSize.y / 2);

    //Asset init
    sf::Image map_tiles;
    map_tiles.loadFromFile("./resources/map_tiles.png");

    sf::Image player_sheet;
    player_sheet.loadFromFile("./resources/player.png");

    sf::Image wheat_sheet;
    wheat_sheet.loadFromFile("./resources/wheat.png");

    sf::Texture map_tile_tex;
    map_tile_tex.loadFromImage(map_tiles, sf::IntRect(0, 0, 32, 32));

    sf::Texture farmable_tex;
    farmable_tex.loadFromImage(map_tiles, sf::IntRect(128, 0, 32, 32));

    std::vector<sf::Texture> player_textures;

    for(int i = 0; i < 4; i++){
        sf::Texture texture;
        texture.loadFromImage(player_sheet, sf::IntRect(i * 32, 0, 32, 64));
        player_textures.push_back(texture);
    }

    std::vector<sf::Texture> wheat_textures;

    for(int i = 0; i < 4; i++){
        sf::Texture texture;
        texture.loadFromImage(wheat_sheet, sf::IntRect(i * 32, 0, 32, 64));
        wheat_textures.push_back(texture);
    }

    std::vector<std::vector<Tile>> map(40, std::vector<Tile>(26));

    for(int i = 0; i < mapSize_grid.x; i++){
        for(int j = 0; j < mapSize_grid.y; j++){
            Tile tile(i * 32, j * 32, map_tile_tex);
            map[i][j] = tile;
        }
    }

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            map[i + 10][j + 10].type = Tile::type::farmable;
            map[i + 10][j + 10].sprite.setTexture(farmable_tex);
        }
    }

    Player player((mapSize_grid.x / 2) * 32, (mapSize_grid.y / 2) * 32, player_textures[2]);
    std::vector<Wheat> crops;
    sf::IntRect testRect(0, 0, 32, 32);
    sf::RectangleShape guideRect(sf::Vector2f(32, 32));
    guideRect.setPosition(0, 0);
    guideRect.setOutlineColor(sf::Color::Red);
    guideRect.setFillColor(transparentColor);
    guideRect.setOutlineThickness(3);
    bool drawGuideRect = false;
    bool canInteract = false;
    int colliderX;
    int colliderY;

    //Game loop
    while(window.isOpen()){
        drawGuideRect = false;
        while(window.pollEvent(event)){
            switch(event.type){
                case sf::Event::EventType::Closed:
                    window.close();
                    break;
                case sf::Event::EventType::KeyPressed:
                    switch(event.key.code){
                        case(sf::Keyboard::Key::Escape):
                            window.close();
                            break;
                        case(sf::Keyboard::Key::E):
                            if(canInteract){
                                if(map[colliderX][colliderY].type == Tile::farmable){
                                    Wheat wheat(colliderX, colliderY, wheat_textures[0]);
                                    crops.push_back(wheat);
                                    map[colliderX][colliderY].type = Tile::busy;
                                }
                            }
                            break;
                        //
                        //Movement
                        //
                        case(sf::Keyboard::Key::W):
                            if(player.direction == player.NORTH)
                                player.sprite.move(0, -32);
                            else{
                                player.sprite.setTexture(player_textures[0]);
                                player.direction = player.NORTH;
                            }
                            break;
                        case(sf::Keyboard::Key::A):
                            if(player.direction == player.WEST)
                                player.sprite.move(-32, 0);
                            else{
                                player.sprite.setTexture(player_textures[3]);
                                player.direction = player.WEST;
                            }
                            break;
                        case(sf::Keyboard::Key::D):
                            if(player.direction == player.EAST)
                                player.sprite.move(32, 0);
                            else{
                                player.sprite.setTexture(player_textures[1]);
                                player.direction = player.EAST;
                            }
                            break;
                        case(sf::Keyboard::Key::S):
                            if(player.direction == player.SOUTH)
                                player.sprite.move(0, 32);
                            else{
                                player.sprite.setTexture(player_textures[2]);
                                player.direction = player.SOUTH;
                            }
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        //Update
        testRect.left = player.sprite.getPosition().x;
        testRect.top = player.sprite.getPosition().y + 32;
        if(player.direction == player.NORTH){
            testRect.top -= 32;
        }
        else if(player.direction == player.EAST){
            testRect.left += 32;
        }
        else if(player.direction == player.SOUTH){
            testRect.top += 32;
        }
        else if(player.direction == player.WEST){
            testRect.left -= 32;
        }

        for(int i = 0; i < mapSize_grid.x; i++){
            for(int j = 0; j < mapSize_grid.y; j++){
                if(map[i][j].type == Tile::farmable){
                    if(map[i][j].tileRect.intersects(testRect)){
                        drawGuideRect = true;
                        colliderX = i;
                        colliderY = j;
                        canInteract = true;
                        break;
                    }
                }
            }
        }


        window.clear(sf::Color::Black);

        for(int i = 0; i < mapSize_grid.x; i++)
            for(int j = 0; j < mapSize_grid.y; j++)
                window.draw(map[i][j].sprite);

        for(const auto& c : crops)
            window.draw(c.sprite);

        window.draw(player.sprite);

        if(drawGuideRect){
            guideRect.setPosition(testRect.left, testRect.top);
            window.draw(guideRect);
        }

        window.display();
    }

    return 1;
}