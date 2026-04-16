#pragma once
#include <cstdint>
#include "SFML/Graphics.hpp"
#include <nlohmann/json.hpp>
#include "TextureManager.h"
#include <format>
#include "Memento.h"

enum class ItemType {
	BONE,
	BRUSH,
	BALL
};

class Item {
public:
	Item(ItemType typeId, uint32_t value, Texture texName, sf::IntRect texRect) 
		: mTypeId(typeId), mValue(value), mTexture(texName), mSprite(TextureManager::getInstance()->getTexture(texName), texRect){}

	Item(const ItemType typeId, const uint32_t value, const sf::Sprite& sprite, const Texture texName, const bool isAlive, const nlohmann::json& state)
		: mTypeId(typeId), mValue(value), mSprite(sprite), mTexture(texName), mIsAlive(isAlive), mState(state) {}

	Item(const Item& other);

	bool operator==(const Item& other) const { return mTypeId == other.getTypeId(); }
	bool operator==(const std::nullptr_t) const { return this == nullptr; }
	bool operator!=(const Item& other) const { return mTypeId != other.getTypeId(); }
	
	virtual std::string getDescription() { return "This is an item."; }
	virtual void printStats() const {}
	void setPosition(sf::Vector2f position) { mSprite.setPosition(position); }
	void setScale(sf::Vector2f scale) { mSprite.setScale(scale); }
	void setAlive(bool value) { mIsAlive = value; }
	void setPicked(bool value) { mIsPicked = value; }
	const bool isAlive() const { return mIsAlive; }
	const bool isPicked() const { return mIsPicked; }
	const ItemType getTypeId() const { return mTypeId; }
	const Texture getTextureName() const { return mTexture; }
	const sf::IntRect getTextureRect() const { return mSprite.getTextureRect(); }
	const sf::Vector2f getScale() const { return mSprite.getScale(); }
	
	sf::Sprite& getSprite() { return mSprite; }
	const uint32_t getValue() const { return mValue; };
	nlohmann::json saveData();

protected:
	uint32_t mValue;
	const ItemType mTypeId;
	bool mIsAlive = true;
	bool mIsPicked = false;
	sf::Sprite mSprite;
	Texture mTexture;
	nlohmann::json mState;

private:
	void setState(nlohmann::json);
	void toJson(nlohmann::json& j);

	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>();
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
};

class Food : public Item {
public:
	Food(ItemType typeId, Texture texName, sf::IntRect texRect, uint32_t value) : Item(typeId, value, texName, texRect) {}
	Food(const Food& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}
	
	std::string getDescription() override{ return std::format("+{} food", mValue); }
	

private:
	
};

class GroomItem : public Item {
public:
	GroomItem(ItemType typeId, Texture texName, sf::IntRect texRect, uint32_t value) : Item(typeId, value, texName, texRect) {}
	GroomItem(const GroomItem& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}

	std::string getDescription() override { return std::format("+{} grooming", mValue); }
	

private:
	
};

class Toy : public Item {
public:
	Toy(ItemType typeId, Texture texName, sf::IntRect texRect, uint32_t value) : Item(typeId, value, texName, texRect) {}
	Toy(const Toy& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}

	std::string getDescription() override { return std::format("+{} happiness", mValue); }
	

private:

};