#include "PetSim.h"





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

    auto food1 = std::make_shared<Food>(1, mSpritesheet, sf::IntRect({ 0,0 }, { 32,32 }), 10);
    mInventorySystem->addItemToSlot({590, 25}, food1);
    mItems.push_back(food1);
    //mRenderItems.push_back(std::static_pointer_cast<sf::Drawable>(food1));

    auto food2 = std::make_shared<GroomItem>(2, mSpritesheet, sf::IntRect({ 32,0 }, { 32,32 }), 10);
    mItems.push_back(food2);
    //mRenderItems.push_back(std::static_pointer_cast<sf::Drawable>(food2));

}

void Game::pollEvents() {

    while (std::optional event = mWindow->pollEvent()) {

        mCurrentEvent = event;

        if (event->is<sf::Event::Closed>() || (event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {

            mWindow->close();
        }
        else  if (event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S) {

            mSaveManager->save();
            mSaveManager->showHistory();
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

        if (!item) continue;
        if (item->getSprite().getGlobalBounds().contains(mouse_position)) {

            mInventorySystem->removeFromSlot(mouse_position, *item);

            while (!mCurrentEvent.value().is<sf::Event::MouseButtonReleased>()) {

                mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*mWindow));
                mInventorySystem->dragItem(mouse_position, *item);
            }

            if (mPet->getSprite().getGlobalBounds().contains(mouse_position)) {

                mNeedsSystem->processItem(*item);
                item.reset();
            }
            else {

                mInventorySystem->addItemToSlot(mouse_position, item);
            }
            
            break;
        }
    }
}

void Game::update(float dt) {
    
    mItems.erase(std::remove(mItems.begin(), mItems.end(), nullptr), mItems.end());

    mNeedsSystem->update(dt);
    mScene->update(dt);
    mInventorySystem->update();
}

void Game::render() {

    mWindow->clear();
    mScene->render(*mWindow);
    mInventorySystem->render(*mWindow);
    for (auto obj : mItems) {

        if (!obj) continue;
        mWindow->draw(obj->getSprite());
    }
    
    mWindow->display();
}

