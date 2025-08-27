#include "InventorySystem.h"

constexpr float INV_MARGIN = 4.0;

InventorySystem::InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition) {
	
	float slot_size_x = ((invDimensions.x - (COLUMNS + 1) * INV_MARGIN) / COLUMNS);
	float slot_size_y = ((invDimensions.y - (ROWS + 1) * INV_MARGIN)/ ROWS);

	size_t i = 0;
	sf::Vector2f cell_position;

	for (auto& column : mInventory) {
		
		for (auto& cell : column) {

			if (i < ROWS) cell_position = { invPosition.x + INV_MARGIN, invPosition.y + INV_MARGIN + i * (slot_size_y + INV_MARGIN) };
			else cell_position = { invPosition.x + slot_size_x + 2*INV_MARGIN, invPosition.y + INV_MARGIN + (i - ROWS) * (slot_size_y + INV_MARGIN) };

			cell.setSize({slot_size_x, slot_size_y});
			cell.setPosition(cell_position);
			cell.setFillColor(sf::Color::Blue);
			i++;
		}
	}
}

void InventorySystem::render(sf::RenderWindow& window) {

	for (const auto& column : mInventory) {

		for (const auto& slot : column) {

			window.draw(slot);
		}
	}
}

const std::tuple<sf::Vector2f, size_t> InventorySystem::getSlotPosition(sf::Vector2i mousePosition) const {

	size_t i = 0;
	for (const auto& column : mInventory) {

		for (const auto& slot : column) {

			if (slot.getGlobalBounds().contains((sf::Vector2f)mousePosition)) {

				return { slot.getPosition(), i };
			}

			i++;
		}
	}
}

void InventorySystem::addItemToInv(sf::Vector2i mousePosition, std::shared_ptr<Item> item) {

	auto [position, index] = getSlotPosition(mousePosition);
	item->setPosition(position);
	mItems[index] = item;
}