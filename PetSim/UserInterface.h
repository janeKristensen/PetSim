#pragma once
#include "SFML//Graphics.hpp"

class ProgressBar {
public:
	ProgressBar(sf::Vector2f size, sf::Vector2f position, sf::Color color) : mFullSize(size){
		mBar.setSize(size);
		mBar.setPosition(position);
		mBar.setFillColor(color);
	}

	void resizeBar(float value) {

		auto x_new = (mFullSize.x / 100) * value;
		mBar.setSize({ x_new, mFullSize.y });
	}

	sf::RectangleShape& getShape() { return mBar; }
	sf::Vector2f getSize() { return mBar.getSize(); }

private:
	sf::RectangleShape mBar;
	sf::Vector2f mFullSize;
};