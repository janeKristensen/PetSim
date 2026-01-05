#include "PetSim.h"
#include "MenuScene.h"


Game::Game(std::shared_ptr<sf::RenderWindow> window) : mWindow(std::move(window)) {

    auto tm = TextureManager::getInstance();
    tm->loadTexture(Texture::SPRITESHEET, "../PetSim/ressources/assets/spritesheet.png");
    tm->loadTexture(Texture::TITLE_MENU, "../PetSim/ressources/assets/title_menu.png");
    tm->loadTexture(Texture::LOADING_SCREEN, "../PetSim/ressources/assets/loading.png");
   
    FontManager::getInstance()->loadFont(FontName::TITLE, "../PetSim/ressources/assets/Gabriola.ttf");
}

void Game::init()
{
    SceneManager::getInstance()->changeScene(std::make_shared<TitleScene>(screenSize, shared_from_this()));
}

void Game::pollEvents() 
{
    while (std::optional event = mWindow->pollEvent()) 
    {
        auto mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mWindow));
        auto scene_manager = SceneManager::getInstance();
        auto scene = scene_manager->getScene();
        scene->setEvent(event);

        if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) 
        {
            mWindow->close();
        }
        else if (event->is<sf::Event::KeyPressed>())
        {
            auto key = event->getIf<sf::Event::KeyPressed>()->code;
            if (key == sf::Keyboard::Key::S) saveGame();
            scene->handleKeyPress(key);
        }
        else if (event->is<sf::Event::MouseButtonReleased>() &&
            event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) 
        {
            scene->handleClick(mouse_position);
        }
        else if (event->is<sf::Event::TextEntered>()) 
        {
            scene->handleTextEntry(*event);
        }
        else if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) 
        {
            mFutures.push_back(std::async(std::launch::async, &Scene::handleDrag, scene, mWindow));
        }

        mFutures.push_back(std::async(std::launch::async, &Scene::handleHover, scene, mouse_position));
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

void Game::loadGame(const std::string& filename)
{
    auto scene_mng = SceneManager::getInstance();
    auto scene = scene_mng->getScene();
    auto menu = dynamic_pointer_cast<MenuScene>(scene);
    if (menu)
    {
        auto game_scene = static_pointer_cast<GameScene>(scene_mng->getPreviousScene());
        game_scene->loadGame(filename);
    }
    else
    {
        mState = scene_mng->getScene()->setState();
    }
}

void Game::quitGame()
{
    mWindow->close();
}


void Game::setState() 
{
    auto scene_mng = SceneManager::getInstance();
    auto scene = scene_mng->getScene();
    auto menu = dynamic_pointer_cast<MenuScene>(scene);
    if (menu)
    {
        auto game_scene = static_pointer_cast<GameScene>(scene_mng->getPreviousScene());
        mState = game_scene->setState();
    }
    else
    {
        mState = scene_mng->getScene()->setState();
    }
    
    mSaveComponent->setState(mState);
}
