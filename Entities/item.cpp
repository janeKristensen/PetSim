#include "item.h"



Item::Item(const Item& other) 
	: mTypeId(other.mTypeId), mValue(other.mValue), mTexture(other.mTexture), mSprite(other.mSprite), mIsAlive(other.mIsAlive), mState(other.mState)
{

}

void Item::toJson(nlohmann::json& j) {

	std::array<float, 6> position;
	auto intRect = (*mSprite).getTextureRect();
	position[0] = intRect.position.x;
	position[1] = intRect.position.y;
	position[2] = intRect.size.x;
	position[3] = intRect.size.y;
	position[4] = (*mSprite).getPosition().x;
	position[5] = (*mSprite).getPosition().y;

	auto scale = (*mSprite).getScale().x;

	j = nlohmann::json{ 
		{"typeId", mTypeId}, 
		{"position", position}, 
		{"value", mValue},
		{"texName", mTexture},
		{"scale", scale}
	};
}

void Item::setState(nlohmann::json) {

	toJson(mState);
	//mSaveComponent->setState(mState);
}

nlohmann::json Item::saveData() {

	setState(mState);
	return mState;
}

void Item::loadData(nlohmann::json data)
{
	// Get item texture rect
	auto arr = data["position"].get<std::array<float, 6>>();
	mTexRect.position.x = arr[0];
	mTexRect.position.y = arr[1];
	mTexRect.size.x = arr[2];
	mTexRect.size.y = arr[3];

	mValue = data["value"].get<uint32_t>();
	mTexture = data["texName"].get<Texture>();
	mSprite = sf::Sprite(mTex, mTexRect);
	float scale = data["scale"].get<float>();
	setScale({ scale, scale });
	setPosition(sf::Vector2f{ arr[4], arr[5] });
}
