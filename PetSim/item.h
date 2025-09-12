#pragma once
#include <cstdint>
#include "SFML/Graphics.hpp"

class Item : public sf::Drawable{
public:
	Item(uint32_t typeId, std::shared_ptr<sf::Texture> texture, sf::IntRect tex_rect) : mTypeId(typeId), mSpritesheet(texture), mSprite(*mSpritesheet, tex_rect){}

	bool operator==(const Item& other) const { return mTypeId == other.getTypeId(); }
	bool operator==(const std::nullptr_t) const { return this == nullptr; }
	bool operator!=(const Item& other) const { return mTypeId != other.getTypeId(); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Sprite& getSprite() { return mSprite; }
	void setPosition(sf::Vector2f position) { mSprite.setPosition(position); }
	const bool isAlive() const { return mIsAlive; }
	void setAlive(bool value) { mIsAlive = value; }
	virtual void printStats() const {}
	virtual const uint32_t getValue() const { return 0; };

private:
	const uint32_t getTypeId() const { return mTypeId; }

	const uint32_t mTypeId;
	bool mIsAlive = true;
	std::shared_ptr<sf::Texture> mSpritesheet;
	sf::Sprite mSprite;
};

class Food : public Item {
public:
	Food(uint32_t typeId, std::shared_ptr<sf::Texture> texture, sf::IntRect texRect, uint32_t feedValue) : mFeedValue(feedValue), Item(typeId, texture, texRect) {}
	Food(const Food& other) : mFeedValue(other.mFeedValue), Item(other){}
	const uint32_t getValue() const override { return mFeedValue; }
	void printStats() const override;

private:
	uint32_t mFeedValue;
};

class GroomItem : public Item {
public:
	GroomItem(uint32_t typeId, std::shared_ptr<sf::Texture> texture, sf::IntRect texRect, uint32_t groomValue) : mGroomValue(groomValue), Item(typeId, texture, texRect) {}
	GroomItem(const GroomItem& other) : mGroomValue(other.mGroomValue), Item(other){}
	const uint32_t getValue() const override{ return mGroomValue; }
	void printStats() const override;

private:
	uint32_t mGroomValue;
};