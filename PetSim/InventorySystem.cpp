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
#ifndef NDEBUG
			cell.setFillColor(sf::Color::Blue);
#endif
			i++;
		}
	}
}

void InventorySystem::render(sf::RenderWindow& window) {

#ifndef NDEBUG
	for (const auto& column : mInventory) {

		for (const auto& slot : column) {

			window.draw(slot);
		}
	}
#endif
}

const std::tuple<sf::Vector2f, size_t> InventorySystem::getSlotPosition(sf::Vector2f mousePosition) const {

	size_t i = 0;
	for (const auto& column : mInventory) {

		for (const auto& slot : column) {

			if (slot.getGlobalBounds().contains(mousePosition)) {

				return { slot.getPosition(), i };
			}

			i++;
		}
	}

	return {};
}

const sf::Vector2f InventorySystem::getSlotPositionAtIndex(size_t index) const {

	// TODO: What if i change the number of columns?
	if (index < ROWS) return mInventory[0][index].getPosition();
	else return mInventory[1][index - ROWS].getPosition();

}

size_t InventorySystem::getFirstEmptySlot() {

	for (size_t i = 0; i < mItems.size(); i++) {

		if (mItems[i] == nullptr) return i;
	}
}

void InventorySystem::addItemToSlot(sf::Vector2f mousePosition, std::shared_ptr<Item> item) {

	auto [position, index] = getSlotPosition(mousePosition);
	if (index == NULL) {
		index = getFirstEmptySlot();
		position = getSlotPositionAtIndex(index);
	}

	item->setPosition(position);
	mItems[index] = item;

}

void InventorySystem::removeFromSlot(Item& item) {

	for (size_t i = 0; i < mItems.size(); i++) {

		if (mItems[i].get() == &item) {
			mItems[i] = nullptr;
		}
	}
}

void InventorySystem::dragItem(const sf::Vector2f mousePosition, Item& item) {

	item.setPosition(mousePosition);
}