#pragma once
#include "Item.h"
#include <array>

constexpr size_t ROWS = 6;
constexpr size_t COLUMNS = 2;

class InventorySystem {
public:
	InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition);
	void render(sf::RenderWindow& window);
	void addItemToInv(sf::Vector2i mousePosition, std::shared_ptr<Item> item);

private:
	const std::tuple<sf::Vector2f, size_t> getSlotPosition(sf::Vector2i mousePosition) const;

	std::array<std::array<sf::RectangleShape, ROWS>, COLUMNS> mInventory;
	std::array<std::shared_ptr<Item>, 12> mItems;
};
