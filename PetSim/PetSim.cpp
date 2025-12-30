#include "PetSim.h"


Game::Game(std::shared_ptr<sf::RenderWindow> window) : mWindow(std::move(window)) {

    auto tm = TextureManager::getInstance();
    tm->loadTexture(Texture::SPRITESHEET, "../ressources/spritesheet.png");
    tm->loadTexture(Texture::TITLE_MENU, "../ressources/title_menu.png");
    tm->loadTexture(Texture::LOADING_SCREEN, "../ressources/loading.png");
   
    FontManager::getInstance()->loadFont(FontName::TITLE, "../ressources/Gabriola.ttf");

    SceneManager::getInstance()->changeScene(std::make_shared<TitleScene>(screenSize));
}


void Game::pollEvents() 
{
    while (std::optional event = mWindow->pollEvent()) 
    {
        auto mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mWindow));
        auto scene = SceneManager::getInstance()->getScene();
        scene->setEvent(event);

        if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) 
        {
            mWindow->close();
        }
        else if (event->is<sf::Event::KeyPressed>())
        {
            auto key = event->getIf<sf::Event::KeyPressed>()->code;
            scene->handleKeyPress(key);
        }
        else if (event->is<sf::Event::MouseButtonReleased>() &&
            event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) 
        {
            SceneManager::getInstance()->getScene()->handleClick(mouse_position);
        }
        else if (event->is<sf::Event::TextEntered>()) 
        {
            scene->handleTextEntry(*event);
        }
        else if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) 
        {
            mFutures.push_back(std::async(std::launch::async, &Scene::handleDrag, SceneManager::getInstance()->getScene(), mWindow));
        }

        mFutures.push_back(std::async(std::launch::async, &Scene::handleHover, SceneManager::getInstance()->getScene(), mouse_position));
    }
}


void Game::update(float dt) 
{
    SceneManager::getInstance()->getScene()->update(dt);
}

void Game::render() 
{
    mWindow->clear();
    SceneManager::getInstance()->getScene()->render(*mWindow);
    mWindow->display();
}

void Game::saveGame() 
{
    setState();
    mSaveManager->save();
    mSaveManager->showHistory();
}


void Game::setState() 
{
    mState = SceneManager::getInstance()->getScene()->setState();
    mSaveComponent->setState(mState);
}
