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

void Item::toJson(nlohmann::json& j, const Item& pet) {

	j = nlohmann::json{ { "typeId", mTypeId }, {"value", mValue}};
}

void Item::setState(nlohmann::json) {

	toJson(mState, *this);
	//mState = {
	//	{"Name", mName},
	//	{"Species", mSpecies},
	//	{"Temper", mTemper},
	//	{"InitPrompt", mInitPrompt},
	//	{ "CurrentStatus", mCurrentStatus },
	//	{"Hunger", mHunger},
	//	{"Groom", mGroom},
	//	{"Health", mHealth},
	//	{"Spritesheet", mSpritesheet},
	//	{"Sprite", mSprite}
	//};
	mSaveComponent->setState(mState);
}

nlohmann::json Item::saveData() {

	setState(mState);
	return mState;
}
