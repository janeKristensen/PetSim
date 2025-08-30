#include "PetSim.h"



constexpr sf::Vector2f screenSize{ 800.f, 600.f };

static std::shared_ptr<Model> makeModel(std::string path, float p, float temp) {

    return std::make_shared<Model>(path, p, temp);
}

Game::Game(sf::Font& font, std::shared_ptr<sf::RenderWindow> window) : mWindow(std::move(window)) {

   
    /* *******************************************************************************
       *   Loading the llm model
       * ****************************************************************************/
    
    std::string model_path = "../llm_model/models/SmolLM2-1.7B-Instruct-IQ4_XS.gguf";
    mModelFuture = std::async(std::launch::async, makeModel, model_path, 0.2, 1.5);

    mSpritesheet = std::make_shared<sf::Texture>("../ressources/spritesheet.png");
    mPet = std::make_shared<Pet>(mSpritesheet, sf::IntRect({ 0,128 }, { 64,64 }), "Kitty", "Cat", "Happy");

    mNeedsSystem = std::make_unique<NeedsSystem>(mPet, mModelFuture);
    mScene = std::make_unique<Scene>(mModelFuture, mSpritesheet, mPet, screenSize, font);
    mInventorySystem = std::make_unique<InventorySystem>(mScene->getInvSize(), mScene->getInvPosition(), font);

    auto food1 = std::make_shared<Food>(mSpritesheet, sf::IntRect({ 0,0 }, { 32,32 }), 10);
    mInventorySystem->addItemToSlot({590, 25}, food1);
    mItems.push_back(std::static_pointer_cast<Item>(food1));
    mRenderItems.push_back(std::static_pointer_cast<sf::Drawable>(food1));

}

void Game::pollEvents() {

    while (std::optional event = mWindow->pollEvent()) {

        mCurrentEvent = event;

        if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {

            mWindow->close();
        }
        else if (event->is<sf::Event::MouseButtonReleased>() &&
            event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {

                auto mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mWindow));
                mScene->handleClick(mouse_position);
        }
        else if (event->is<sf::Event::TextEntered>()) {

            mScene->handleTextEntry(*event);
        }
        else if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {

            if (mItems.empty()) return;
            mFutures.push_back(std::async(std::launch::async, &Game::handleDrag, this));
        }
    }
}

void Game::handleDrag() {

    auto mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mWindow));

    for (auto& item : mItems) {

        if (item->getSprite().getGlobalBounds().contains(mouse_position)) {

            mInventorySystem->removeFromSlot(*item);

            while (!mCurrentEvent.value().is<sf::Event::MouseButtonReleased>()) {

                mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mWindow));
                mInventorySystem->dragItem(mouse_position, *item);
            }
            mInventorySystem->addItemToSlot(mouse_position, item);
            break;
        }
    }
}

void Game::update(float dt) {
    
    mNeedsSystem->update(dt);
    mScene->update(dt);
    mInventorySystem->update();
}

void Game::render() {

    mWindow->clear();
    mScene->render(*mWindow);
    mInventorySystem->render(*mWindow);
    for (auto obj : mRenderItems) {

        auto item = static_pointer_cast<Item>(obj);
        mWindow->draw(item->getSprite());
    }
    
    mWindow->display();
}

