#include "PetSim.h"



constexpr float DECAY_TIME = 60.0f;
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

    mScene = std::make_unique<Scene>(mModelFuture, mSpritesheet, mPet, screenSize, font);

    auto food1 = std::make_shared<Food>(mSpritesheet, sf::IntRect({ 0,0 }, { 32,32 }), 10);
    mItems.push_back(std::static_pointer_cast<Item>(food1));
    mRenderItems.push_back(std::static_pointer_cast<sf::Drawable>(food1));

}

void Game::update(float dt) {

    mTimeTracker += dt;

    if (mTimeTracker >= DECAY_TIME) {

        mTimeTracker = 0;
        mPet->decayValues();

        auto hunger = mPet->getHungerValue();
        if (hunger == 80) {
            mModel->addSystemPrompt("A little hungry.");
        }
        else if (hunger == 40) {

            mModel->addSystemPrompt("Very hungry and want food.");
        }
    }

    mScene->update(dt);
}

void Game::render() {

    mWindow->clear();
    mScene->render(*mWindow);
    for (auto obj : mRenderItems) {

        auto item = static_pointer_cast<Item>(obj);
        mWindow->draw(item->getSprite());
    }
    
    mWindow->display();
}

