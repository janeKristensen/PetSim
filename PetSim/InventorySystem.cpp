#include "InventorySystem.h"

constexpr float INV_MARGIN = 2.0;

InventorySystem::InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition) {

	
	float slot_size_x = (invDimensions.x / 2) - 2 * INV_MARGIN;
	float slot_size_y = (invDimensions.y / 6) - 2 * INV_MARGIN;

	size_t i = 0;
	sf::Vector2f cell_position;

	for (auto& column : mInventory) {
		
		for (auto& cell : column) {

			if (i < 6) {
				cell_position = { invPosition.x + INV_MARGIN, invPosition.y + INV_MARGIN + i * (slot_size_y + 2 * INV_MARGIN) };
			}
			else cell_position = { invPosition.x + slot_size_x + 2 * INV_MARGIN, invPosition.y + INV_MARGIN + (i - 6) * (slot_size_y + 2 * INV_MARGIN) };

			cell.setSize({slot_size_x, slot_size_y});
			cell.setPosition(cell_position);
			cell.setFillColor(sf::Color::Blue);
			i++;
		}
	}

}

void InventorySystem::render(sf::RenderWindow& window) {

	for (const auto& column : mInventory) {

		for (const auto& cell : column) {

			window.draw(cell);
		}
	}
}