#pragma once
#include "Item.h"
#include <array>


class InventorySystem {
public:
	InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition);
	void render(sf::RenderWindow& window);

private:
	std::array<std::array<sf::RectangleShape, 6>, 2> mInventory;
};
