#include "item.h"



void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const{

	target.draw(mSprite, states);
}

void Food::printStats() const {

	std::string stats = std::format("+{} food", mValue);
}

void GroomItem::printStats() const {

	std::string stats = std::format("+{} grooming", mValue);
}

void Item::toJson(nlohmann::json& j) {

	std::array<float, 6> position;
	auto intRect = mSprite.getTextureRect();
	position[0] = intRect.position.x;
	position[1] = intRect.position.y;
	position[2] = intRect.size.x;
	position[3] = intRect.size.y;
	position[4] = mSprite.getPosition().x;
	position[5] = mSprite.getPosition().y;

	j = nlohmann::json{ 
		{"typeId", mTypeId}, 
		{"position", position}, 
		{"value", mValue}
	};
}

void Item::setState(nlohmann::json) {

	toJson(mState);
	mSaveComponent->setState(mState);
}

nlohmann::json Item::saveData() {

	setState(mState);
	return mState;
}
