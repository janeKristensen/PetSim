#pragma once
#include <future>
#include "SFML/Graphics.hpp"
#include "Item.h"
#include "Pet.h"
#include "Scene.h"
#include "Model.h"
#include "Memento.h"
#include "NeedsSystem.h"
#include "InventorySystem.h"
#include <fstream>

constexpr sf::Vector2f screenSize{ 800.f, 600.f };

static std::shared_ptr<Model> makeModel(std::string path, float p, float temp) {

	return std::make_shared<Model>(path, p, temp);
}

class Game {
public:
	Game(sf::Font& font, std::shared_ptr<sf::RenderWindow> window);
	void init();
	void pollEvents();
	void update(float dt);
	void render();
	std::shared_ptr<Scene> getScene() { return mScene; }
	void saveGame();
	void loadGame(const std::string& filename);

private:
	void handleDrag();
	void setState();
	std::shared_ptr<Item> createItemFromType(const ItemType type, sf::IntRect texRect, uint32_t value);

	nlohmann::json mState;
	std::vector<std::shared_ptr<Item>> mItems;
	std::vector<std::shared_ptr<sf::Drawable>> mRenderItems;
	std::vector<std::future<void>> mFutures;
	std::shared_ptr<sf::Texture> mSpritesheet = nullptr;
	std::unique_ptr<NeedsSystem> mNeedsSystem;
	std::unique_ptr<InventorySystem> mInventorySystem;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>(); 
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
	std::shared_ptr<Scene> mScene = nullptr;
	std::shared_ptr<Pet> mPet = nullptr;
	std::shared_ptr<Model> mModel = nullptr;
	std::shared_ptr<sf::RenderWindow> mWindow = nullptr;
	std::shared_future<std::shared_ptr<Model>> mModelFuture;
	std::optional<sf::Event> mCurrentEvent;
};
