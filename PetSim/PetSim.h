#pragma once
#include <future>
#include "SFML/Graphics.hpp"
#include "Item.h"
#include "Pet.h"
#include "Scene.h"
#include "Model.h"
#include "NeedsSystem.h"
#include "InventorySystem.h"


class Game {
public:
	Game(sf::Font& font, std::shared_ptr<sf::RenderWindow> window);
	void pollEvents();
	void update(float dt);
	void render();
	std::shared_ptr<Scene> getScene() { return mScene; }

private:
	void handleDrag();

	std::vector<std::shared_ptr<Item>> mItems;
	std::vector<std::shared_ptr<sf::Drawable>> mRenderItems;
	std::vector<std::future<void>> mFutures;
	std::shared_ptr<sf::Texture> mSpritesheet = nullptr;
	std::unique_ptr<NeedsSystem> mNeedsSystem;
	std::unique_ptr<InventorySystem> mInventorySystem;
	std::shared_ptr<Scene> mScene = nullptr;
	std::shared_ptr<Pet> mPet = nullptr;
	std::shared_ptr<Model> mModel = nullptr;
	std::shared_ptr<sf::RenderWindow> mWindow = nullptr;
	std::shared_future<std::shared_ptr<Model>> mModelFuture;
	std::optional<sf::Event> mCurrentEvent;
};
