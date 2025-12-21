#pragma once
#include "Item.h"
#include <array>
#include "TextureManager.h"
#include "FontManager.h"
#include <iostream>

constexpr size_t ROWS = 6;
constexpr size_t COLUMNS = 2;
constexpr size_t MAX_SLOTS = ROWS * COLUMNS;

class Slot {
public:
	sf::RectangleShape& getShape() { return mShape; }
	const sf::RectangleShape& getShape() const { return mShape; }
	void clear() { mAmount = 0; }
	const int32_t getAmount() const { return mAmount; }
	void setAmount(int32_t value) { 
		mAmount += value; 
		if (mAmount < 0) mAmount = 0; 
	}

private:
	sf::RectangleShape mShape;
	int32_t mAmount;
};



class InventorySystem {
public:
	InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition, Texture texName);
	void update();
	void render(sf::RenderWindow& window);
	const std::tuple<sf::Vector2f, int32_t> getSlotPosition(sf::Vector2f mousePosition) const;
	void addItemToSlot(sf::Vector2f mousePosition, std::shared_ptr<Item> item);
	void addItemToSlotIndex(size_t index, std::shared_ptr<Item> item, int amount);
	void removeFromSlot(sf::Vector2f mousePosition, Item& item);
	void removeFromSlotIndex(size_t index, Item& item);
	void dragItem(const sf::Vector2f mousePosition, Item& item);
	void clearSlots();
	nlohmann::json saveData();

private:
	const std::tuple<size_t, size_t> getRowColumnIndex(size_t index) const;
	const sf::Vector2f getSlotPositionAtIndex(size_t index) const;
	size_t getFirstEmptySlot();
	void adjustItemCount(int32_t value, size_t index);
	std::shared_ptr<Item> spawnItem(const Item& item);
	void despawnItem(std::shared_ptr<Item> item);

	std::array<std::array<Slot, COLUMNS>, ROWS> mInventory;
	std::vector<std::shared_ptr<Item>> mItems{ MAX_SLOTS, nullptr };
	Texture mTexture;
	std::vector<sf::Text> mAmountText;
	sf::Vector2f mSlotSize;
	nlohmann::json mState;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>();
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
	void setState(nlohmann::json);
	void toJson(nlohmann::json& j);
};
