#include "UserInterface.h"


ShopTile::ShopTile(sf::Vector2f size, sf::Vector2f position, std::tuple<std::unique_ptr<Item>, uint32_t>& item, const sf::Font& font)
	: mItem(item), mPriceText(sf::Text(font, std::format("{}", std::get<1>(mItem)))),
	mDescription(sf::Text(font, std::get<0>(mItem)->getDescription()))
{
	float TEXT_MARGIN = 20.f;

	mTile.setPosition(position);
	mTile.setSize(size);
	mTile.setFillColor(sf::Color::Magenta);
	mTile.setOutlineColor(sf::Color::Red);
	mHoverTile.setPosition(position * 1.2f);
	mDescription.setPosition({mHoverTile.getPosition().x + TEXT_MARGIN, mHoverTile.getPosition().y});
	mHoverTile.setSize({200,50});
	mHoverTile.setFillColor(sf::Color::Black);
	std::get<0>(mItem)->setScale({5,5});
	std::get<0>(mItem)->setPosition(position);
	mPriceText.setPosition({position.x + TEXT_MARGIN, position.y + mTile.getSize().y - mPriceText.getCharacterSize() - TEXT_MARGIN });
}

void ShopTile::update()
{
	mIsSelected ? mTile.setOutlineThickness(5) : mTile.setOutlineThickness(0);
}

void ShopTile::render(sf::RenderWindow& window)
{
	window.draw(mTile);
	window.draw(std::get<0>(mItem)->getSprite());
	window.draw(mPriceText);
	if (mIsHovering)
	{
		window.draw(mHoverTile);
		window.draw(mDescription);
	}
}

Item ShopTile::getItem() 
{ 
	Item item = *std::get<0>(mItem);
	return item;
}