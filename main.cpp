#include <sstream>
#include <string>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "include/tile.hpp"
#include "include/player.hpp"
#include "include/wheat.hpp"
#include "include/seedbag.hpp"
#include "include/cropbox.hpp"
#include "include/bed.hpp"

template<typename T>
std::string toString(T arg){
    std::stringstream ss;
    ss << arg;
    return ss.str();
}

enum gameStates{
    PLAY,
    SLEEP,
    PAUSE
};

int main(){
    //Window init
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Kisetsu no Uta - Prototype");
    window.setFramerateLimit(60);
    sf::Event event;
    sf::Color transparentColor(0,0,0,0);

    sf::Vector2i mapSize(1280, 736);
    sf::Vector2i mapSize_grid(mapSize.x / 32, mapSize.y / 32);

    //View stuff
    auto windowSize = window.getSize();
    sf::View playerView(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    sf::View hudView(sf::FloatRect(0, 0, windowSize.x, windowSize.y));

    playerView.setCenter(mapSize.x / 2, mapSize.y / 2);
    window.setView(playerView);

    //Clock stuff
    sf::Clock clock;
    sf::Time deltaTime;
    sf::Time elapsedTime;
    srand(time(NULL));

    //Ingame clock stuff
    sf::Font font;
    font.loadFromFile("./resources/font.ttf");
    sf::Text clock_text("", font);
    clock_text.setCharacterSize(12);
    clock_text.setPosition(10, 10);
    clock_text.setFillColor(sf::Color::White);

    sf::Text resource_text("0", font);
    resource_text.setCharacterSize(12);
    resource_text.setPosition(10, window.getSize().y - 128);
    resource_text.setFillColor(sf::Color::White);

    sf::Text cropBox_text("0", font);
    cropBox_text.setCharacterSize(12);
    cropBox_text.setPosition(0,0);
    cropBox_text.setFillColor(sf::Color::White);

    sf::Time ingameTimer;

    int minutes = 0;
    int hours = 6;
    int days = 1, season = 1, year = 1;

    int money = 500;

    //Overlays
    sf::RectangleShape pauseOverlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::RectangleShape sleepOverlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::Color pauseColor(50, 50, 50, 40);
    sf::Color sleepColor(50, 50, 200, 0);
    sf::Text pauseText("Paused", font);
    pauseText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    //Asset init
    sf::Image map_tiles;
    map_tiles.loadFromFile("./resources/map_tiles.png");

    sf::Image player_sheet;
    player_sheet.loadFromFile("./resources/player.png");

    sf::Image wheat_sheet;
    wheat_sheet.loadFromFile("./resources/wheat.png");

    sf::Image seed_bag_sheet;
    seed_bag_sheet.loadFromFile("./resources/seed_bag.png");

    sf::Image crop_box_sheet;
    crop_box_sheet.loadFromFile("./resources/crop_box.png");

    sf::Image bed_sheet;
    bed_sheet.loadFromFile("./resources/bed.png");

    sf::Texture bed_tex;
    bed_tex.loadFromImage(bed_sheet, sf::IntRect(0, 0, 32, 64));

    sf::Texture seed_bag_tex;
    seed_bag_tex.loadFromImage(seed_bag_sheet, sf::IntRect(0, 0, 32, 32));

    sf::Texture crop_box_tex;
    crop_box_tex.loadFromImage(crop_box_sheet, sf::IntRect(0, 0, 32, 32));

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

    CropBox cropBox(256 * 2, 64, crop_box_tex);
    cropBox_text.setPosition(cropBox.sprite.getPosition().x, cropBox.sprite.getPosition().y - 10);
    SeedBag seedBag(320 * 2, 64, seed_bag_tex);
    Bed bed(356 * 2, 64, bed_tex);

    sf::IntRect testRect(0, 0, 32, 32);
    sf::RectangleShape guideRect(sf::Vector2f(32, 32));
    guideRect.setPosition(0, 0);
    guideRect.setOutlineColor(sf::Color::Red);
    guideRect.setFillColor(transparentColor);
    guideRect.setOutlineThickness(3);
    bool drawGuideRect = false;
    bool canInteract = false;
    bool canFarm = false;
    //0 = seedBag, 1 = cropBox, 2 = bed
    std::vector<bool> colFlags;
    colFlags.push_back(false);
    colFlags.push_back(false);
    colFlags.push_back(false);

    int colliderX;
    int colliderY;

    //Game loop
    int state = PLAY;
    while(window.isOpen()){
        //Time
        deltaTime = clock.restart();
        float dtAsSeconds = deltaTime.asSeconds();
        elapsedTime += deltaTime;

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
                            if(state == PLAY){
                                if(canFarm){
                                    if(map[colliderX][colliderY].type == Tile::farmable && player.seeds >= 1){
                                        Wheat wheat(colliderX, colliderY, wheat_textures);
                                        crops.push_back(wheat);
                                        map[colliderX][colliderY].type = Tile::busy;
                                        player.seeds--;
                                    }
                                    else if(map[colliderX][colliderY].type == Tile::busy){
                                        for(int i = 0; i < crops.size(); i++){
                                            if(crops[i].mapX == colliderX && crops[i].mapY == colliderY && crops[i].state == crops[i].harvestable){
                                                player.crops++;
                                                crops.erase(crops.begin() + i);
                                                map[colliderX][colliderY].type == Tile::farmable;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if(canInteract){
                                    if(colFlags[0]){
                                        if(money >= 50){
                                            player.seeds++;
                                            money -= 50;
                                        }
                                    }
                                    if(colFlags[1]){
                                        if(player.crops > 0){
                                            cropBox.current_crops += player.crops;
                                            cropBox.crop_history += player.crops;
                                            player.crops = 0;
                                        }
                                    }
                                    if(colFlags[2]){
                                        if(state == PLAY)
                                            state = SLEEP;
                                    }
                                }
                            }
                            break;
                        case(sf::Keyboard::Key::BackSpace):
                            if(state == PLAY)
                                state = PAUSE;
                            else if(state == PAUSE)
                                state = PLAY;
                            break;
                        //
                        //Movement
                        //
                        case(sf::Keyboard::Key::W):
                            if(state == PLAY){
                                if(player.direction == player.NORTH)
                                    player.sprite.move(0, -32);
                                else{
                                    player.sprite.setTexture(player_textures[0]);
                                    player.direction = player.NORTH;
                                }
                            }
                            break;
                        case(sf::Keyboard::Key::A):
                            if(state == PLAY){
                                if(player.direction == player.WEST)
                                    player.sprite.move(-32, 0);
                                else{
                                    player.sprite.setTexture(player_textures[3]);
                                    player.direction = player.WEST;
                                }
                            }
                            break;
                        case(sf::Keyboard::Key::D):
                            if(state == PLAY){
                                if(player.direction == player.EAST)
                                    player.sprite.move(32, 0);
                                else{
                                    player.sprite.setTexture(player_textures[1]);
                                    player.direction = player.EAST;
                                }                                
                            }
                            break;
                        case(sf::Keyboard::Key::S):
                            if(state == PLAY){
                                if(player.direction == player.SOUTH)
                                    player.sprite.move(0, 32);
                                else{
                                    player.sprite.setTexture(player_textures[2]);
                                    player.direction = player.SOUTH;
                                }                                
                            }
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        switch(state){
            case PLAY:
                //Update
                for(int i = 0; i < 3; i++)
                    colFlags[i] = false;

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
                //Reset the collision flag here so we aren't able to interact when away from the tile
                canInteract = false;
                canFarm = false;

                for(int i = 0; i < mapSize_grid.x; i++){
                    for(int j = 0; j < mapSize_grid.y; j++){
                        if(map[i][j].type == Tile::farmable || map[i][j].type == Tile::busy){
                            if(map[i][j].tileRect.intersects(testRect)){
                                drawGuideRect = true;
                                colliderX = i;
                                colliderY = j;
                                canFarm = true;
                                break;
                            }
                        }
                    }
                }

                if(seedBag.colRect.intersects(testRect)){
                    drawGuideRect = true;
                    canInteract = true;
                    colFlags[0] = true;
                }
                if(cropBox.colRect.intersects(testRect)){
                    drawGuideRect = true;
                    canInteract = true;
                    colFlags[1] = true; 
                }
                if(bed.colRect.intersects(testRect)){
                    drawGuideRect = true;
                    canInteract = true;
                    colFlags[2] = true;
                }

                //Clock update
                ingameTimer += deltaTime;
                if(ingameTimer.asSeconds() >= 5){
                    minutes += 10;
                    ingameTimer = sf::seconds(0);
                }
                if(minutes >= 60){
                    hours++;
                    minutes = 0;
                }
                if(hours >= 24){
                    days++;
                    hours = 0;
                }
                if(days > 28){
                    season++;
                    days = 1;
                }
                if(season > 4){
                    year++;
                    season = 1;
                }

                clock_text.setString("Elapsed time: " + toString<int>(elapsedTime.asSeconds())
                                    + "\n" + toString<int>(hours) + " : "  + toString<int>(minutes)
                                    + "\n" + toString<int>(days) + " of " + toString<int>(season) + " " + toString<int>(year) + " year");

                resource_text.setString("Money: " + toString<int>(money)
                                    + "\n" + "Seeds: " + toString<int>(player.seeds)
                                    + "\n" + "Crops: " + toString<int>(player.crops));

                cropBox_text.setString(toString<int>(cropBox.current_crops));

                //Render world
                window.clear(sf::Color::Black);

                for(int i = 0; i < mapSize_grid.x; i++)
                    for(int j = 0; j < mapSize_grid.y; j++)
                        window.draw(map[i][j].sprite);

                for(const auto& c : crops)
                    window.draw(c.sprite);

                window.draw(seedBag.sprite);
                window.draw(cropBox.sprite);
                window.draw(bed.sprite);

                if(drawGuideRect){
                    guideRect.setPosition(testRect.left, testRect.top);
                    window.draw(guideRect);
                }

                window.draw(player.sprite);

                //Render HUD
                window.setView(hudView);
                window.draw(clock_text);
                window.draw(resource_text);
                window.draw(cropBox_text);

                break;
            case SLEEP:
                if(sleepColor.a >= 254){
                    sleepColor.a = 0;
                    state = PLAY;
                    money += cropBox.current_crops * 69;
                    cropBox.current_crops = 0;
                    for(int i = 0; i < crops.size(); i++)
                        crops[i].update(wheat_textures);
                    break;
                }

                sleepColor.a += 2;
                sleepOverlay.setFillColor(sleepColor);
                window.clear(sf::Color::Black);

                window.setView(playerView);

                for(int i = 0; i < mapSize_grid.x; i++)
                    for(int j = 0; j < mapSize_grid.y; j++)
                        window.draw(map[i][j].sprite);

                for(auto const& c : crops)
                    window.draw(c.sprite);

                window.draw(player.sprite);

                window.setView(hudView);

                window.draw(sleepOverlay);

                break;
            case PAUSE:
                window.setView(hudView);
                window.clear(pauseColor);
                window.draw(pauseText);
                break;
            default:
                break;
        }

        window.display();
    }

    return 1;
}