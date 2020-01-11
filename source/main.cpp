#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "../include/player.hpp"

int main(){
    
    //Window initialization
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Kisetsu no Uta - Prototype");
    window.setFramerateLimit(60);

    sf::Vector2i mapSize(1280, 720);
    sf::Vector2i viewPosition(mapSize.x / 2, mapSize.y / 2);

    //Clock text
    sf::Font font;
    font.loadFromFile("./resources/font.ttf");

    sf::Text clock_text("funt", font);
    sf::String clock_string;

    clock_text.setPosition(10, 10);
    clock_text.setFillColor(sf::Color::White);

    //Time init
    sf::Time deltaTime;
    sf::Clock clock;
    srand(time(NULL));

    //Asset init
    sf::Image player_image;
    player_image.loadFromFile("./resources/player.png");

    sf::Texture player_tex;
    player_tex.loadFromImage(player_image, sf::IntRect(0, 0, 32, 64));

    //View initialization
    auto windowSize = window.getSize();
    float zoom = 1;
    sf::View view(sf::FloatRect(0, 0, windowSize.x, windowSize.y));

    view.setCenter(viewPosition.x, viewPosition.y);
    window.setView(view);

    //Game init
    Player player(mapSize.x / 2, mapSize.y / 2, player_tex);

    //Game loop
    while(window.isOpen()){
        //Time
        deltaTime = clock.restart();
        float dtAsSeconds = deltaTime.asSeconds();

        //Event handling || Input handling
        sf::Event event;
        while(window.pollEvent(event)){
           switch(event.type){
                case sf::Event::EventType::Closed:
                    window.close();
                    break;
                case sf::Event::EventType::KeyPressed:
                    break;
                case sf::Event::EventType::KeyReleased:
                    if(event.key.code == sf::Keyboard::Key::Escape)
                        window.close();
                        break;
                    switch(event.key.code){
                        case(sf::Keyboard::Key::W):
                            player.velocity.y = 0;
                            break;
                        case(sf::Keyboard::Key::S):
                            player.velocity.y = 0;
                            break;
                        case(sf::Keyboard::Key::A):
                            player.velocity.x = 0;
                            break;
                        case(sf::Keyboard::Key::D):
                        player.velocity.x = 0;
                        break;
                    }
                    break;
                default:
                    break;
           } 
        }

        //Update

        //Clock update
        
        clock_text.setString(clock_string);

        //Clunky player movement dunno how to make nice tbh
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            player.velocity.y = -10;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            player.velocity.y = 10;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            player.velocity.x = -10;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            player.velocity.x = 10;

        view.setCenter(viewPosition.x, viewPosition.y);
        window.setView(view);

        player.sprite.move(player.velocity.x, player.velocity.y);

        //Render
        window.clear(sf::Color::Green);

        window.draw(player.sprite);
        window.draw(clock_text);

        window.display();
    }

    return 0;
}