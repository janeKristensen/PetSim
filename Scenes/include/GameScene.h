#pragma once
#include <future>
#include "Model.h"
#include "scene.h"
#include "UserInterface.h"
#include "Pet.h"


static void pushRequestToModel(std::string query, std::shared_ptr<Model> model) { model->userQuery(query); }

class GameScene : public Scene {

public:
	GameScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, std::shared_ptr<Model> model);
	~GameScene();

	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
	void setModel(std::shared_ptr<Model> model);
	void handleClick(sf::Vector2f mouseposition) override;
	void handleKeyPress(sf::Keyboard::Key key) override;
	void handleDrag(std::shared_ptr<sf::RenderWindow> window) override;
	void asyncDrag(std::shared_ptr<sf::RenderWindow> window);
	void handleTextEntry(const sf::Event& event) override;
	nlohmann::json setState() override;
	void loadGame(const std::string& filename) override;
	void addItemToInventory(std::shared_ptr<Item> item, std::uint32_t amount);
	// Move to item manager
	std::shared_ptr<Item> createItemFromType(const ItemType type, Texture texName, sf::IntRect texRect, uint32_t value);

private:
	std::string getPrompt();
	void setCurrentPet(std::shared_ptr<Pet> pet) { mPet = pet; mPet->setSpritePosition(mPetPosition); }
	void addToStringBuffer(const char text) { mStringBuffer += text; }
	void eraseFromStringBuffer();
	bool isInTextField() { return mInTextField; }

	

	std::vector<std::shared_ptr<Item>> mItems;
	std::vector<std::shared_ptr<Item>> mItemsToAdd;
	std::unique_ptr<NeedsSystem> mNeedsSystem;
	std::unique_ptr<InventorySystem> mInventorySystem;
	std::shared_ptr<Pet> mPet = nullptr;
	std::shared_ptr<Model> mModel = nullptr;
	std::vector<std::future<void>> mFutures;
	ProgressBar mFoodBar;
	ProgressBar mGroomBar;
	std::string mStringBuffer;
	std::string mStringLine;
	sf::Vector2f mPetPosition;
	sf::Vector2f mScreenSize;
	float mItemScale;
	uint32_t mMoney = 0;
	bool mInTextField = false;
	float mBlipTracker = 0.f;
	float mResponseTracker = 0.f;
};
