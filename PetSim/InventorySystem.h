#pragma once
#include "Item.h"
#include <array>

constexpr size_t ROWS = 6;
constexpr size_t COLUMNS = 2;
constexpr size_t MAX_SLOTS = ROWS * COLUMNS;

class InventorySystem {
public:
	InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition);
	void render(sf::RenderWindow& window);
	void addItemToSlot(sf::Vector2f mousePosition, std::shared_ptr<Item> item);
	void removeFromSlot(Item& item);
	void dragItem(const sf::Vector2f mousePosition, Item& item);

private:
	const std::tuple<sf::Vector2f, size_t> getSlotPosition(sf::Vector2f mousePosition) const;
	const sf::Vector2f getSlotPositionAtIndex(size_t index) const;
	size_t getFirstEmptySlot();

	std::array<std::array<sf::RectangleShape, ROWS>, COLUMNS> mInventory;
	std::vector<std::shared_ptr<Item>> mItems{ MAX_SLOTS, nullptr };
};
