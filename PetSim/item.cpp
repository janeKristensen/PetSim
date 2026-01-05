#include "item.h"



Item::Item(const Item& other) 
	: mTypeId(other.mTypeId), mValue(other.mValue), mTexture(other.mTexture), mSprite(other.mSprite), mIsAlive(other.mIsAlive), mState(other.mState)
{

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
		{"value", mValue},
		{"texName", mTexture}
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
