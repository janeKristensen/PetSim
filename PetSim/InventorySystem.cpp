#include "InventorySystem.h"


constexpr float INV_MARGIN = 4.0;


InventorySystem::InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition, const sf::Font& font) : mFont(font){
	
	
	float slot_size_x = ((invDimensions.x - (COLUMNS + 1) * INV_MARGIN) / COLUMNS);
	float slot_size_y = ((invDimensions.y - (ROWS + 1) * INV_MARGIN)/ ROWS);

	mAmountText.reserve(MAX_SLOTS * sizeof(sf::Text));
	sf::Vector2f slot_position;

	size_t i = 0;
	for (auto& column : mInventory) {
		
		for (auto& slot : column) {

			if (i < ROWS) slot_position = { invPosition.x + INV_MARGIN, invPosition.y + INV_MARGIN + i * (slot_size_y + INV_MARGIN) };
			else slot_position = { invPosition.x + slot_size_x + 2*INV_MARGIN, invPosition.y + INV_MARGIN + (i - ROWS) * (slot_size_y + INV_MARGIN) };

			auto& slot_rect = slot.getShape();
			slot_rect.setSize({slot_size_x, slot_size_y});
			slot_rect.setPosition(slot_position);
#ifndef NDEBUG
			slot_rect.setFillColor(sf::Color::Blue);
#endif
			slot.setAmount(0);

			mAmountText.push_back(sf::Text(font, "", 36));
			auto char_size = mAmountText[i].getCharacterSize();
			auto text_position_x = slot_position.x + INV_MARGIN;
			auto text_position_y = slot_position.y + slot_size_y - char_size - INV_MARGIN;
			mAmountText[i].setPosition({text_position_x, text_position_y});
			mAmountText[i].setFillColor(sf::Color::White);
			mAmountText[i].setStyle(sf::Text::Bold);
			mAmountText[i].setString("");
			i++;
		}
	}	
}

void InventorySystem::update() {

	size_t i = 0;
	for (const auto& column : mInventory) {

		for (const auto& slot : column) {

			int32_t item_amount = slot.getAmount();
			if(item_amount > 0) mAmountText[i].setString(std::to_string(item_amount));
			else mAmountText[i].setString("");
			i++;
		}
	}
}


void InventorySystem::render(sf::RenderWindow& window) {

	size_t i = 0;
	for (const auto& column : mInventory) {

		for (const auto& slot : column) {
#ifndef NDEBUG
			window.draw(slot.getShape());
#endif	
			auto& text = mAmountText[i];
			if (text.getString() != "") {
				window.draw(text);
			}
			i++;
		}
	}

}

const std::tuple<sf::Vector2f, int32_t> InventorySystem::getSlotPosition(sf::Vector2f mousePosition) const {

	size_t i = 0;
	for (const auto& column : mInventory) {

		for (const auto& slot : column) {

			auto& slot_rect = slot.getShape();
			if (slot_rect.getGlobalBounds().contains(mousePosition)) {

				return { slot_rect.getPosition(), i };
			}

			i++;
		}
	}

	return { {}, -1 };
}

const sf::Vector2f InventorySystem::getSlotPositionAtIndex(size_t index) const {

	if (index > MAX_SLOTS) {

		return {};
	}

	size_t row = index / COLUMNS;
	size_t col = index % COLUMNS;
	return mInventory[row][col].getShape().getPosition();
}

size_t InventorySystem::getFirstEmptySlot() {

	for (size_t i = 0; i < mItems.size(); i++) {

		if (mItems[i] == nullptr) return i;
	}
}

void InventorySystem::adjustItemCount(int32_t value, size_t index) {

	if (index > MAX_SLOTS) {

		return;
	}

	size_t row = index / COLUMNS;
	size_t col = index % COLUMNS;
	mInventory[row][col].setAmount(value);
}

void InventorySystem::addItemToSlot(sf::Vector2f mousePosition, std::shared_ptr<Item> item) {

	auto [position, index] = getSlotPosition(mousePosition);

	// if not dragged inside an inventory slot; check if there is an item af same type in inventory
	if (index == -1) {

		for (int i = 0; i < mItems.size(); i++) {

			if (*mItems[i] == *item) {
				index = i;
				position = getSlotPositionAtIndex(i);
				break;
			}
		}
	} // if not, find next empty slot.
	else if (mItems[index] && *mItems[index] != *item) {
		index = getFirstEmptySlot();
		position = getSlotPositionAtIndex(index);
	}


	item->setPosition(position);
	mItems[index] = item;
	adjustItemCount(1, index);
	
	size_t row = index / COLUMNS;
	size_t col = index % COLUMNS;
	if (mInventory[row][col].getAmount() > 1) {

		despawnItem(item);
	}
}

void InventorySystem::removeFromSlot(sf::Vector2f mousePosition, Item& item) {

	auto [position, index] = getSlotPosition(mousePosition);

	if (index == -1 || !mItems[index]) return;

	adjustItemCount(-1, index);
	size_t row = index / COLUMNS;
	size_t col = index % COLUMNS;
	uint32_t item_amount = mInventory[row][col].getAmount();

	if(item_amount == 0) mItems[index] = nullptr;
	else if(item_amount >= 1) { mItems[index] = spawnItem(item); }		
}

void InventorySystem::dragItem(const sf::Vector2f mousePosition, Item& item) {

	item.setPosition(mousePosition);
}

void InventorySystem::despawnItem(std::shared_ptr<Item> item) {

	item.reset();	
	std::cout << "Despawned item" << std::endl;
}

std::shared_ptr<Item> InventorySystem::spawnItem(const Item& item) {

	auto new_item = std::make_shared<Item>(item);
	std::cout << "Spawned item" << std::endl;
	return new_item;
}