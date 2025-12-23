#pragma once
#include "SFML//Graphics.hpp"
#include "FontManager.h"
#include "item.h"



class ProgressBar {
public:
	ProgressBar() {}
	
	ProgressBar(sf::Vector2f size, sf::Vector2f position, sf::Color color) : mFullSize(size)
	{
		mBar.setSize(size);
		mBar.setPosition(position);
		mBar.setFillColor(color);
	}
	

	void resizeBar(float value) 
	{
		auto x_new = (mFullSize.x / 100) * value;
		mBar.setSize({ x_new, mFullSize.y });
	}

	sf::RectangleShape& getShape() { return mBar; }
	sf::Vector2f getSize() { return mBar.getSize(); }
	sf::Vector2f getFullSize() { return mFullSize; }

private:
	sf::RectangleShape mBar;
	sf::Vector2f mFullSize;
};


class ShopTile {
public:
	ShopTile(sf::Vector2f size, sf::Vector2f position, std::tuple<std::unique_ptr<Item>, uint32_t>& item);
	void render(sf::RenderWindow& window);
	void update();
	void setHoverState(bool state) { mIsHovering = state; }
	void selectTile(bool state) { mIsSelected = state; };
	sf::FloatRect getBounds() { return mTile.getGlobalBounds(); }
private:
	sf::RectangleShape mTile;
	sf::RectangleShape mHoverTile;
	uint32_t mPrice = 0;
	std::tuple<std::unique_ptr<Item>, uint32_t>& mItem;
	sf::Text mPriceText;
	sf::Text mDescription;
	bool mIsHovering = false;
	bool mIsSelected = false;
};