#pragma once
#include <cstdint>
#include "SFML/Graphics.hpp"

class Item : public sf::Drawable{
public:
	Item(std::shared_ptr<sf::Texture> texture, sf::IntRect tex_rect) : mSpritesheet(std::move(texture)), mSprite(*mSpritesheet, tex_rect){}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void printStats() const {}
	sf::Sprite& getSprite() { return mSprite; }

private:
	std::shared_ptr<sf::Texture> mSpritesheet;
	sf::Sprite mSprite;
};

class Food : public Item {
public:
	Food(std::shared_ptr<sf::Texture> texture, sf::IntRect texRect, uint32_t feedValue) : mFeedValue(feedValue), Item(texture, texRect) {}
	const uint32_t getFeedValue() const { return mFeedValue; }
	void printStats() const override;
private:
	uint32_t mFeedValue;
};

class GroomItem : public Item {
public:
	GroomItem(std::shared_ptr<sf::Texture> texture, sf::IntRect texRect, uint32_t groomValue) : mGroomValue(groomValue), Item(texture, texRect) {}
	const uint32_t getFeedValue() const { return mGroomValue; }
	void printStats() const override;
private:
	uint32_t mGroomValue;
};