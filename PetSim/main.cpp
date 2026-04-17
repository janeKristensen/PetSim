#include <GL/glew.h>
#include "PetSim.h"
#include "SFML/OpenGL.hpp"
#include <iostream>


int main()
{
    try {

        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.majorVersion = 3;
        settings.minorVersion = 3;

        auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode({ 1020, 660 }), "PetSim");
        
        window->setVerticalSyncEnabled(true);
        window->setActive(true);

        glewExperimental = GL_TRUE; // helps with core profiles
        GLenum err = glewInit();
        
        if (err != GLEW_OK) {
            std::cout << "GLEW error: " << glewGetErrorString(err) << std::endl;
        }
        

        std::shared_ptr<Game> game = std::make_shared<Game>(window);
        game->init();

        sf::Clock clock;
        float dt = 0.0f;
        
        bool running = true;
        while (running)
        {
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
