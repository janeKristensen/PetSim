#include "ItemManager.h"



std::shared_ptr<Item> ItemManager::createItemFromType(ItemType type)
{
	std::shared_ptr<Item> item = nullptr;

	if (type == ItemType::BONE)
	{
		item = std::make_shared<Food>(type);
	}
	else if (type == ItemType::BRUSH)
	{
		item = std::make_shared<GroomItem>(type);
	}
	else if (type == ItemType::BALL)
	{
		item = std::make_shared<Toy>(type);
	}
	else if (type == ItemType::NUMBER)
	{
		item = std::make_shared<Number>(type);
	}

	return item;
}

std::shared_ptr<Item> ItemManager::createItemFromTypeTemplate(ItemType type, const Texture textureName, const sf::IntRect& textureRect, const uint32_t value)
{
	auto item = createItemFromType(type);
	item->setTextureName(textureName);
	item->setTextureRect(textureRect);
	item->setValue(value);

	return item;
}

