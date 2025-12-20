#include "InventorySystem.h"


constexpr float INV_MARGIN = 0.0;


InventorySystem::InventorySystem(sf::Vector2f invDimensions, sf::Vector2f invPosition, Texture texName) 
	: mTexture(texName){
	
	float slot_size_x = ((invDimensions.x - (COLUMNS + 1) * INV_MARGIN) / COLUMNS);
	float slot_size_y = ((invDimensions.y - (ROWS + 1) * INV_MARGIN)/ ROWS);

	mAmountText.reserve(MAX_SLOTS * sizeof(sf::Text));
	sf::Vector2f slot_position;

	size_t i = 0;
	for (auto& column : mInventory) {
		
		for (auto& slot : column) {

			if (i < ROWS) slot_position = { 
				invPosition.x, 
				invPosition.y + i * slot_size_y 
			};
			else slot_position = { 
				invPosition.x + slot_size_x, 
				invPosition.y +  (i - ROWS) * slot_size_y 
			};

			auto& slot_rect = slot.getShape();
			slot_rect.setSize({slot_size_x, slot_size_y});
			slot_rect.setPosition(slot_position);
			slot_rect.setTexture(&TextureManager::getInstance()->getTexture(mTexture));
			slot_rect.setTextureRect(sf::IntRect({159,0}, {32,32}));
			slot.setAmount(0);

			auto& font = FontManager::getInstance()->getFont(FontName::TITLE);
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

const std::tuple<size_t, size_t> InventorySystem::getRowColumnIndex(size_t index) const {

	assert(index <= MAX_SLOTS);

	size_t row = index / COLUMNS;
	size_t col = index % COLUMNS;

	return std::tuple<size_t, size_t>(row, col);
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

	auto [row, col] = getRowColumnIndex(index);
	return mInventory[row][col].getShape().getPosition();
}

size_t InventorySystem::getFirstEmptySlot() {

	for (size_t i = 0; i < mItems.size(); i++) {

		if (mItems[i] == nullptr) return i;
	}

	return -1;
}

void InventorySystem::adjustItemCount(int32_t value, size_t index) {

	auto [row, col] = getRowColumnIndex(index);
	mInventory[row][col].setAmount(value);
}

void InventorySystem::addItemToSlotIndex(size_t index, std::shared_ptr<Item> item, int amount) {

	auto position = getSlotPositionAtIndex(index);
	item->setPosition(position);
	adjustItemCount(amount, index);
	mItems[index] = item;
}

void InventorySystem::addItemToSlot(sf::Vector2f mousePosition, std::shared_ptr<Item> item) {

	auto [position, index] = getSlotPosition(mousePosition);

	// if not dragged inside an inventory slot
	if (index == -1) {

		// check if there is an item of same type in inventory
		for (int i = 0; i < mItems.size(); i++) {
			
			if (*mItems[i] != nullptr && *mItems[i] == *item) {
				index = i;
				position = getSlotPositionAtIndex(index);
				break;
			}
		}

		// if not, find next empty slot.
		if (index == -1) {
			
			index = getFirstEmptySlot();
			position = getSlotPositionAtIndex(index);
		}
	} 
	else if (*mItems[index] != nullptr && *mItems[index] != *item) {

		// if item in slot not same type, find next empty slot.
		index = getFirstEmptySlot();
		position = getSlotPositionAtIndex(index);
	}

	// Set to postion of index
	item->setPosition(position);
	mItems[index] = item;
	adjustItemCount(1, index);
	
	// only one item instance lives in the inventory slot
	auto [row, col] = getRowColumnIndex(index);
	if (mInventory[row][col].getAmount() > 1) {

		despawnItem(item);
	}
}

void InventorySystem::removeFromSlotIndex(size_t index, Item& item) {

	adjustItemCount(-1, index);
	auto [row, col] = getRowColumnIndex(index);
	uint32_t item_amount = mInventory[row][col].getAmount();

	if (item_amount == 0) mItems[index].reset();
	else if (item_amount >= 1) { mItems[index] = spawnItem(item); }
}

void InventorySystem::removeFromSlot(sf::Vector2f mousePosition, Item& item) {

	auto [position, index] = getSlotPosition(mousePosition);
	if (index == -1 || !mItems[index]) return;
	removeFromSlotIndex(index, item);
}

void InventorySystem::dragItem(const sf::Vector2f mousePosition, Item& item) {

	item.setPosition(mousePosition);
}

void InventorySystem::despawnItem(std::shared_ptr<Item> item) {

#ifndef NDEBUG
	std::cout << "Despawned item" << std::endl;
#endif

	item.reset();	
}

std::shared_ptr<Item> InventorySystem::spawnItem(const Item& item) {

#ifndef NDEBUG
	std::cout << "Spawned item" << std::endl;
#endif

	auto new_item = std::make_shared<Item>(item);
	return new_item;
}

void InventorySystem::clearSlots() {

	// clear inventory from items without deallocating memory
	for (auto item : mItems) {

		item = nullptr;
	}

	for (int i = 0; i < mInventory.size(); i++) {
		for (int j = 0; j < mInventory[i].size(); j++) {

			mInventory[i][j].clear();
		}
	}
}

void InventorySystem::toJson(nlohmann::json& j) {

	std::array<int, MAX_SLOTS> values;
	int index = 0;

	for (int i = 0; i < mInventory.size(); i++) {
		for (int j = 0; j < mInventory[i].size(); j++) {

			values[index] = mInventory[i][j].getAmount();
			index++;
		}
	}
	
	j = nlohmann::json{ { "slotValues", values } };
}

void InventorySystem::setState(nlohmann::json) {

	toJson(mState);
	mSaveComponent->setState(mState);
}

nlohmann::json InventorySystem::saveData() {

	setState(mState);
	return mState;
}

