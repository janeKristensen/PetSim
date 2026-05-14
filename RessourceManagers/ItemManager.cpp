#include "ItemManager.h"



std::shared_ptr<Item> ItemManager::createItemFromType(const ItemType type, Texture texName, sf::IntRect texRect, uint32_t value)
{
	std::shared_ptr<Item> item = nullptr;

	if (type == ItemType::BONE)
	{
		item = std::make_shared<Food>(type, texName, texRect, *mServices.textureManager->getTexture(texName), value);
	}
	else if (type == ItemType::BRUSH)
	{
		item = std::make_shared<GroomItem>(type, texName, texRect, *mServices.textureManager->getTexture(texName), value);
	}
	else if (type == ItemType::BALL)
	{
		item = std::make_shared<Toy>(type, texName, texRect, *mServices.textureManager->getTexture(texName), value);
	}

	return item;
}

