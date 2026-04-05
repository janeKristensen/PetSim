#include "PetSim.h"
#include <iostream>

int main()
{
    try {

        auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 800, 600 }), "PetSim");

        std::shared_ptr<Game> game = std::make_shared<Game>(window);
        game->init();

        sf::Clock clock;
        float dt = 0.0f;
        

        while (window->isOpen()) {
            
            dt = clock.getElapsedTime().asSeconds();
            clock.restart();
 
            game->pollEvents();
            game->update(dt);
            game->render();
            
        }
    }
    catch (std::exception) {

        std::cout << "Fatal error, exiting..." << std::endl;
    }

    return 0;
}
