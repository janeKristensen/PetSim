#pragma once
#include "SFML/Graphics.hpp"
#include "Command.h"
#include "FontManager.h"
#include <memory>
#include "item.h"

class Command;

class Button : public sf::RectangleShape{
public:
	Button(sf::Vector2f size) : sf::RectangleShape(size) {}
	Button(sf::Vector2f size, std::shared_ptr<Command> command) : sf::RectangleShape(size), mCommand(command) {}

	void setCommand(std::shared_ptr<Command> command) { mCommand = command; }
	std::shared_ptr<Command> getCommand() { return mCommand; }
	void onClick() { mCommand->execute(); }

private:
	std::shared_ptr<Command> mCommand = nullptr;
};

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
	void setPosition(float position) { mBar.setPosition({position, mBar.getPosition().y });
}

private:
	sf::RectangleShape mBar;
	sf::Vector2f mFullSize;
};


class ShopTile {
public:
	ShopTile(sf::Vector2f size, sf::Vector2f position, std::tuple<std::unique_ptr<Item>, uint32_t>& item, const sf::Font& font);
	void render(sf::RenderWindow& window);
	void update();
	void setHoverState(bool state) { mIsHovering = state; }
	void selectTile(bool state) { mIsSelected = state; };
	Item getItem();
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