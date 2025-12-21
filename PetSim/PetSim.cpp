#include "PetSim.h"


Game::Game(std::shared_ptr<sf::RenderWindow> window) : mWindow(std::move(window)) {

    TextureManager::getInstance()->loadTexture(Texture::SPRITESHEET, "../ressources/spritesheet.png");
    TextureManager::getInstance()->loadTexture(Texture::TITLE_MENU, "../ressources/title_menu.png");
    FontManager::getInstance()->loadFont(FontName::TITLE, "../ressources/Gabriola.ttf");

    SceneManager::getInstance()->addScene(std::make_shared<Menu_Scene>(screenSize));
}


void Game::pollEvents() {

    while (std::optional event = mWindow->pollEvent()) {

        auto scene = SceneManager::getInstance()->getScene();
        scene->setEvent(event);

        if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {

            mWindow->close();
        }
        else if (event->is<sf::Event::KeyPressed>()){
            
            auto key = event->getIf<sf::Event::KeyPressed>()->code;
            if (key == sf::Keyboard::Key::S) {

                saveGame();
            }
            else if (key == sf::Keyboard::Key::Q) {
                scene->loadGame("pretty.json");
            }
        }
        else if (event->is<sf::Event::MouseButtonReleased>() &&
            event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {

                auto mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mWindow));
                scene->handleClick(mouse_position);
        }
        else if (event->is<sf::Event::TextEntered>()) {

            scene->handleTextEntry(*event);
        }
        else if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {

            mFutures.push_back(std::async(std::launch::async, &Scene::handleDrag, scene, mWindow));
        }
    }
}


void Game::update(float dt) {
    
    SceneManager::getInstance()->getScene()->update(dt);
}

void Game::render() {

    mWindow->clear();
    SceneManager::getInstance()->getScene()->render(*mWindow);
    mWindow->display();
}

void Game::saveGame() {
    
    setState();
    mSaveManager->save();
    mSaveManager->showHistory();
}


void Game::setState() 
{
    mState = SceneManager::getInstance()->getScene()->setState();
    mSaveComponent->setState(mState);
}
