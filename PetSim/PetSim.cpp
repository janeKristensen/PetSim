#include "PetSim.h"
#include "MenuScene.h"


Game::Game(std::shared_ptr<sf::RenderWindow> window) : mWindow(std::move(window)) {

    srand(time(0));

    auto tm = TextureManager::getInstance();
    tm->loadTexture(Texture::SPRITESHEET, "ressources/assets/spritesheet.png");
    tm->loadTexture(Texture::TITLE_MENU, "ressources/assets/title_menu.png");
    tm->loadTexture(Texture::LOADING_SCREEN, "ressources/assets/loading.png");

    FontManager::getInstance()->loadFont(FontName::TITLE, "ressources/fonts/Gabriola.ttf");

    auto sm = SoundManager::getInstance();
    sm->loadSound(Sound::CLICK, "Sounds/click.wav");
}

void Game::init()
{
    SceneManager::getInstance()->changeScene(std::make_shared<TitleScene>((sf::Vector2f)mWindow->getSize(), shared_from_this()));
}

void Game::pollEvents() 
{
    while (std::optional event = mWindow->pollEvent()) 
    {
        sf::Vector2i pixel_pos = sf::Mouse::getPosition(*mWindow);
        sf::Vector2f mouse_position = mWindow->mapPixelToCoords(pixel_pos);
        auto scene_manager = SceneManager::getInstance();
        auto scene = scene_manager->getScene();
        scene->setEvent(event);

        if (event->is<sf::Event::Closed>()) 
        {
            mWindow->close();
        }
        else if (event->is<sf::Event::Resized>())
        {
            auto newSize = event->getIf<sf::Event::Resized>()->size;
            sf::FloatRect visibleArea({ 0.f, 0.f }, { (float)newSize.x, (float)newSize.y});
            mWindow->setView(sf::View(visibleArea));
        }
        else if (event->is<sf::Event::KeyPressed>())
        {
            auto key = event->getIf<sf::Event::KeyPressed>()->code;
            scene->handleKeyPress(key);
        }
        else if (event->is<sf::Event::MouseButtonReleased>() &&
            event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) 
        {
            mMouseDown = false;

            if (!mIsDragging)
            {
                scene->handleClick(mouse_position);
            }
            else 
            {
                mIsDragging = false;
            }
        }
        else if (event->is<sf::Event::TextEntered>()) 
        {
            scene->handleTextEntry(*event);
        }
        else if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) 
        {
            mMouseDownPosition = mouse_position;
            mMouseDown = true;
            if (std::dynamic_pointer_cast<GameScene>(scene))
            {
                mFutures.push_back(std::async(std::launch::async, &Scene::handleDrag, scene, mWindow));
                mMouseDown = false;
            }
        }

        mFutures.push_back(std::async(std::launch::async, &Scene::handleHover, scene, mouse_position));
    }
}


void Game::update(float dt) 
{
    auto sm = SceneManager::getInstance();
    auto scene = sm->getScene();

    if (mMouseDown && !mIsDragging)
    {
        sf::Vector2i pixel_pos = sf::Mouse::getPosition(*mWindow);
        sf::Vector2f mouse_position = mWindow->mapPixelToCoords(pixel_pos);
        float distance = std::hypot(mouse_position.x - mMouseDownPosition.x,
            mouse_position.y - mMouseDownPosition.y);

        if (distance > 5.0f) // threshold in pixels
        {
            mIsDragging = true; 
        }
    }
    if (mIsDragging)
    {
        if (std::dynamic_pointer_cast<GameScene>(scene))
        {
            
        }
        else 
        {
            scene->handleDrag(mWindow);
        }   
    }
    scene->update(dt);
    SoundManager::getInstance()->update();
   
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
