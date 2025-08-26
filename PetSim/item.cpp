#include "item.h"
#include <format>



void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const{

	target.draw(mSprite, states);
}

void Food::printStats() const {

	std::string stats = std::format("+{} food", mFeedValue);
}

void GroomItem::printStats() const {

	std::string stats = std::format("+{} grooming", mGroomValue);
}