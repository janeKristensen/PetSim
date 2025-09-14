#pragma once
#include <cstdint>
#include "SFML/Graphics.hpp"
#include <nlohmann/json.hpp>
#include <format>
#include "Memento.h"

class Item : public sf::Drawable{
public:
	Item(uint32_t typeId, uint32_t value, std::shared_ptr<sf::Texture> texture, sf::IntRect tex_rect) : mTypeId(typeId), mValue(value), mSpritesheet(texture), mSprite(*mSpritesheet, tex_rect){}
	Item(const Item& other) : mTypeId(other.mTypeId), mValue(other.mValue), mSpritesheet(other.mSpritesheet), mSprite(other.mSprite) {}

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
	const uint32_t getTypeId() const { return mTypeId; }
	void setState(nlohmann::json);
	void toJson(nlohmann::json& j, const Item& pet);

	const uint32_t mTypeId;
	bool mIsAlive = true;
	std::shared_ptr<sf::Texture> mSpritesheet;
	sf::Sprite mSprite;
	nlohmann::json mState;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>();
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
};

class Food : public Item {
public:
	Food(uint32_t typeId, std::shared_ptr<sf::Texture> texture, sf::IntRect texRect, uint32_t value) : Item(typeId, value, texture, texRect) {}
	void printStats() const override;

private:
	
};

class GroomItem : public Item {
public:
	GroomItem(uint32_t typeId, std::shared_ptr<sf::Texture> texture, sf::IntRect texRect, uint32_t value) : Item(typeId, value, texture, texRect) {}
	void printStats() const override;

private:

};