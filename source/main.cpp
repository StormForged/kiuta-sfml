#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <sstream>
#include <string>

#include "../include/player.hpp"
#include "../include/wheat.hpp"

template<typename T>
std::string toString(T arg){
    std::stringstream ss;
    ss << arg;
    return ss.str();
}

enum game_states{
    sleep,
    play,
    pause
};

int main(){ 
    //Window initialization
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Kisetsu no Uta - Prototype");
    window.setFramerateLimit(60);

    sf::Vector2i mapSize(2560, 1440);
    sf::Vector2i viewPosition(mapSize.x / 2, mapSize.y / 2);

    sf::Font font;
    font.loadFromFile("./resources/font.ttf");

    sf::RectangleShape pauseOverlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::RectangleShape sleepOverlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::Color pauseColor(50, 50, 50, 40);
    sf::Color sleepColor(50, 50, 200, 0);
    sf::Text pauseText("Paused", font);
    pauseText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    //Time init
    sf::Time deltaTime;
    sf::Time elapsedTime;
    sf::Clock clock;
    srand(time(NULL));

    //ingame clock stuff
    sf::Text clock_text("funt", font);
    clock_text.setPosition(10, 10);
    clock_text.setFillColor(sf::Color::White);

    sf::Time ingameTimer;

    int minutes = 0;
    int hours = 6;
    int days = 1;
    int season = 1;
    int year = 1; 

    //Asset init
    sf::Image player_image;
    player_image.loadFromFile("./resources/player.png");

    sf::Image map_tiles;
    map_tiles.loadFromFile("./resources/map_tiles.png");

    sf::Image wheat_tiles;
    wheat_tiles.loadFromFile("./resources/wheat.png");

    sf::Texture player_tex;
    player_tex.loadFromImage(player_image, sf::IntRect(0, 0, 32, 64));

    sf::Texture grass_texture1;
    grass_texture1.loadFromImage(map_tiles, sf::IntRect(0, 0, 32, 32));

    sf::Texture grass_texture2;
    grass_texture2.loadFromImage(map_tiles, sf::IntRect(32, 0, 32, 32));

    sf::Texture grass_texture3;
    grass_texture3.loadFromImage(map_tiles, sf::IntRect(64, 0, 32, 32));

    sf::Texture grass_texture4;
    grass_texture4.loadFromImage(map_tiles, sf::IntRect(96, 0, 32, 32));

    sf::Texture wheat_texture1;
    wheat_texture1.loadFromImage(wheat_tiles, sf::IntRect(0, 0, 32, 64));

    sf::Texture wheat_texture2;
    wheat_texture2.loadFromImage(wheat_tiles, sf::IntRect(32, 0, 32, 64));

    sf::Texture wheat_texture3;
    wheat_texture3.loadFromImage(wheat_tiles, sf::IntRect(64, 0, 32, 64));

    sf::Texture wheat_texture4;
    wheat_texture4.loadFromImage(wheat_tiles, sf::IntRect(96, 0, 32, 64));

    //View initialization
    auto windowSize = window.getSize();
    float zoom = 1;
    sf::View view(sf::FloatRect(0, 0, windowSize.x, windowSize.y));
    sf::View hudView(sf::FloatRect(0, 0, windowSize.x, windowSize.y));

    view.setCenter(viewPosition.x, viewPosition.y);
    window.setView(view);

    //Game init
    Player player(mapSize.x / 2, mapSize.y / 2, player_tex);

    std::vector<std::vector<sf::Sprite>> tiles(80, std::vector<sf::Sprite>(45));
    std::vector<Wheat> wheat;
    
    for(int i = 0; i < 80; i++){
        for(int j = 0; j < 45; j++){
            int tileNum = rand() % 10 + 1;
            sf::Sprite sprite;
            if(tileNum == 1)
                sprite.setTexture(grass_texture4);
            else if(tileNum == 2)
                sprite.setTexture(grass_texture3);
            else if(tileNum == 3)
                sprite.setTexture(grass_texture2);
            else
                sprite.setTexture(grass_texture1);

            tiles[i][j] = sprite;
            tiles[i][j].setPosition(i * 32, j * 32);
            tiles[i][j].setOrigin(16, 16);           
        }
    }

    int wheat_num = 0;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            wheat_num++;
            Wheat wheatObj(wheat_texture1);
            wheatObj.sprite.setPosition(1028 + (32* i), 128 + (32* j));
            wheat.push_back(wheatObj);
        }
    }

    //Game loop
    int state = 1;
    while(window.isOpen()){
        //Time
        deltaTime = clock.restart();
        float dtAsSeconds = deltaTime.asSeconds();
        elapsedTime += deltaTime;

        //Event handling || Input handling
        sf::Event event;
        while(window.pollEvent(event)){
           switch(event.type){
                case sf::Event::EventType::Closed:
                    window.close();
                    break;
                case sf::Event::EventType::KeyPressed:
                    switch(event.key.code){
                        case(sf::Keyboard::Key::U):
                            if(state == play)
                                minutes += 10;
                            break;
                        case(sf::Keyboard::Key::I):
                            if(state == play)
                                hours++;
                            break;
                        case(sf::Keyboard::Key::O):
                            if(state == play)
                                days++;
                            break;
                        case(sf::Keyboard::Key::P):
                            if(state == play)
                                season++;
                            break;
                        case(sf::Keyboard::Key::E):
                            if(state == play){
                                for(int i = 0; i < wheat.size(); i++)
                                wheat[i].days_planted++;
                            }
                            break;
                        case(sf::Keyboard::Key::Q):
                            state = sleep;
                            break;
                        default:
                            break;
                    }
                    break;
                case sf::Event::EventType::KeyReleased:
                    if(event.key.code == sf::Keyboard::Key::Escape)
                        window.close();
                    switch(event.key.code){
                        case(sf::Keyboard::Key::W):
                            if(state == play)                     
                                player.velocity.y = 0;
                            break;
                        case(sf::Keyboard::Key::S):
                            if(state == play)
                                player.velocity.y = 0;
                            break;
                        case(sf::Keyboard::Key::A):
                            if(state == play)
                                player.velocity.x = 0;
                            break;
                        case(sf::Keyboard::Key::D):
                            if(state == play)
                                player.velocity.x = 0;
                            break;
                        case(sf::Keyboard::Key::BackSpace):
                            if(state == play){
                                pauseOverlay.setPosition(player.sprite.getPosition().x - (windowSize.x / 2), player.sprite.getPosition().y - (windowSize.y / 2));
                                state = pause;
                            }
                            else if(state != play && state != sleep)
                                state = play;
                            break;
                    }
                    break;
                default:
                    break;
           } 
        }

        //Update
        //State machine time??
        switch(state){
            case sleep:
                if(sleepColor.a >= 254){
                    sleepColor.a = 0;
                    state = play;
                    for(int i = 0; i < wheat.size(); i++)
                        wheat[i].days_planted++;
                    break;
                }

                sleepColor.a += 2;
                sleepOverlay.setFillColor(sleepColor);
                window.clear(sf::Color::Black);

                window.setView(view);

                for(int i = 0; i < 80; i++)
                    for(int j = 0; j < 45; j++)
                        window.draw(tiles[i][j]);

                for(auto const& w : wheat)
                    window.draw(w.sprite);

                window.draw(player.sprite);

                window.setView(hudView);

                window.draw(sleepOverlay);

                break;
            case play:
                for(int i = 0; i < wheat.size(); i++){
                    if(wheat[i].days_planted < 4){
                        if(wheat[i].days_planted == 1)
                            wheat[i].sprite.setTexture(wheat_texture2);
                        if(wheat[i].days_planted == 2)
                            wheat[i].sprite.setTexture(wheat_texture3);
                        if(wheat[i].days_planted == 3)
                            wheat[i].sprite.setTexture(wheat_texture4);
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
                                    + "\n" + toString<int>(hours) + " : " + toString<int>(minutes)
                                    + "\n" + toString<int>(days) + " of " + toString<int>(season) + ", " + toString<int>(year) + " year");


                //Clunky player movement dunno how to make nice tbh
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    player.velocity.y = -10;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    player.velocity.y = 10;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                    player.velocity.x = -10;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                    player.velocity.x = 10;

                view.setCenter(player.sprite.getPosition().x, player.sprite.getPosition().y);
                window.setView(view);
                player.sprite.move(player.velocity.x, player.velocity.y);

                //Render
                window.clear(sf::Color::Black);

                for(int i = 0; i < 80; i++)
                    for(int j = 0; j < 45; j++)
                        window.draw(tiles[i][j]);

                for(auto const& w : wheat)
                    window.draw(w.sprite);

                window.draw(player.sprite);

                //Drawing over game world
                window.setView(hudView);
                window.draw(clock_text);
            break;
            case pause:
                window.setView(hudView);
                window.clear(pauseColor);
                window.draw(pauseText);
                //window.draw(pauseOverlay);
                break;
            default:
                break;
        }

        window.display();
    }

    return 0;
}