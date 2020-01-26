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
    clock_text.setPosition(10, 10);
    clock_text.setFillColor(sf::Color::White);

    sf::Time ingameTimer;

    int minutes = 0;
    int hours = 6;
    int days = 1, season = 1, year = 1;

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
                                if(canInteract){
                                    if(map[colliderX][colliderY].type == Tile::farmable){
                                        Wheat wheat(colliderX, colliderY, wheat_textures);
                                        crops.push_back(wheat);
                                        map[colliderX][colliderY].type = Tile::busy;
                                    }
                                }
                            }
                            break;
                        case(sf::Keyboard::Key::Q):
                            if(state == PLAY)
                                state = SLEEP;
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

                //Render world
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

                //Render HUD
                window.setView(hudView);
                window.draw(clock_text);

                break;
            case SLEEP:
                if(sleepColor.a >= 254){
                    sleepColor.a = 0;
                    state = PLAY;
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