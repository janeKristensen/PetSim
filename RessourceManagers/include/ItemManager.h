#pragma once
#include "item.h"
#include <memory>


class ItemManager
{
public:
	std::shared_ptr<Item> createItemFromType(ItemType type);
	std::shared_ptr<Item> createItemFromTypeTemplate(ItemType type, const Texture textureName, const sf::IntRect& textureRect, const uint32_t value);
private:

};