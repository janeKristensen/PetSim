#pragma once
#include "Item.h"
#include <array>
#include <iostream>

constexpr size_t ROWS = 6;
constexpr size_t COLUMNS = 2;
constexpr size_t MAX_SLOTS = ROWS * COLUMNS;

class Slot {
public:
	sf::RectangleShape& getShape() { return mShape; }
	const sf::RectangleShape& getShape() const { return mShape; }
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
	InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition, const sf::Font& font);
	void update();
	void render(sf::RenderWindow& window);
	void addItemToSlot(sf::Vector2f mousePosition, std::shared_ptr<Item> item);
	void removeFromSlot(sf::Vector2f mousePosition, Item& item);
	void dragItem(const sf::Vector2f mousePosition, Item& item);

private:;
	const std::tuple<sf::Vector2f, int32_t> getSlotPosition(sf::Vector2f mousePosition) const;
	const sf::Vector2f getSlotPositionAtIndex(size_t index) const;
	size_t getFirstEmptySlot();
	void adjustItemCount(int32_t value, size_t index);
	void despawnItem(std::shared_ptr<Item> item);
	std::shared_ptr<Item> spawnItem(const Item& item);

	std::array<std::array<Slot, COLUMNS>, ROWS> mInventory;
	std::vector<std::shared_ptr<Item>> mItems{ MAX_SLOTS, nullptr };
	std::vector<sf::Text> mAmountText;
	const sf::Font& mFont;

};
