#pragma once
#include "item.h"


class ItemManager
{
public:
	std::shared_ptr<Item> createItemFromType(const ItemType type, Texture texName, sf::IntRect texRect, uint32_t value);
private:

};