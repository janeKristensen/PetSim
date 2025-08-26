#include "PetSim.h"
#include <iostream>

int main()
{
    try {

        auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 800, 600 }), "PetSim");
        window->setFramerateLimit(60);

        sf::Font font("../ressources/Gabriola.ttf");
        Game game(font, window);

        sf::Clock clock;
        float dt;
        

        while (window->isOpen()) {

            dt = clock.getElapsedTime().asSeconds();
            clock.restart();
            auto scene = game.getScene();

            while (std::optional event = window->pollEvent()) {

                if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() &&
                    event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {

                    window->close();
                }
                else if (event->is<sf::Event::MouseButtonReleased>() &&
                    event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {

                        auto mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
                        scene->handleClick(mouse_position);
                     }

                if (scene->isInTextField()) {

                    if (event->is<sf::Event::TextEntered>()) {

                        auto unicode = event->getIf<sf::Event::TextEntered>()->unicode;
                        if (unicode == 8) {

                            scene->eraseFromStringBuffer();
                        }
                        else if (unicode < 128) {

                            scene->addToStringBuffer(static_cast<char>(unicode));
                        }
                    }                    
                }           
            }

            game.update(dt);
            game.render();

        }
    }
    catch (std::exception) {

        std::cout << "Fatal error, exiting..." << std::endl;
    }


    return 0;
}
