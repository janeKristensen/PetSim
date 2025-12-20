#pragma once
#include <cstdint>
#include "SFML/Graphics.hpp"
#include <nlohmann/json.hpp>
#include "TextureManager.h"
#include <format>
#include "Memento.h"

enum class ItemType {
	FOOD,
	GROOM
};

class Item : public sf::Drawable{
public:
	Item(ItemType typeId, uint32_t value, Texture texName, sf::IntRect tex_rect) 
		: mTypeId(typeId), mValue(value), mTexture(texName), mSprite(TextureManager::getInstance()->getTexture(texName), tex_rect){}
	Item(const Item& other) : mTypeId(other.mTypeId), mValue(other.mValue), mTexture(other.mTexture), mSprite(other.mSprite) {}

	bool operator==(const Item& other) const { return mTypeId == other.getTypeId(); }
	bool operator==(const std::nullptr_t) const { return this == nullptr; }
	bool operator!=(const Item& other) const { return mTypeId != other.getTypeId(); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Sprite& getSprite() { return mSprite; }
	void setPosition(sf::Vector2f position) { mSprite.setPosition(position); }
	const bool isAlive() const { return mIsAlive; }
	void setAlive(bool value) { mIsAlive = value; }
	virtual void printStats() const {}
	const uint32_t getValue() const { return mValue; };
	nlohmann::json saveData();

protected:
	uint32_t mValue;

private:
	const ItemType getTypeId() const { return mTypeId; }
	void setState(nlohmann::json);
	void toJson(nlohmann::json& j);

	const ItemType mTypeId;
	bool mIsAlive = true;
	sf::Sprite mSprite;
	Texture mTexture;
	nlohmann::json mState;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>();
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
};

class Food : public Item {
public:
	Food(ItemType typeId, Texture tex_name, sf::IntRect texRect, uint32_t value) : Item(typeId, value, tex_name, texRect) {}
	void printStats() const override;

private:
	
};

class GroomItem : public Item {
public:
	GroomItem(ItemType typeId, Texture tex_name, sf::IntRect texRect, uint32_t value) : Item(typeId, value, tex_name, texRect) {}
	void printStats() const override;

private:

};