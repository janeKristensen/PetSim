#include "Pet.h"
#include <iostream>

constexpr int32_t MAX_VALUE = 100;
constexpr int32_t MIN_VALUE = 0;
constexpr int32_t LIMIT_VALUE = 50;
constexpr int32_t DECAY_VALUE = -1;

Pet::Pet(Texture texName, sf::IntRect texRect, std::string name, std::string species, std::string temper) 
	: mSprite(TextureManager::getInstance()->getTexture(texName), texRect), mTexture(texName), mName(name), mSpecies(species), mTemper(temper) {
	mInitPrompt = std::format("Pretend to be a {} named {}. You are my pet not a chatbot. Give short and cute replies to messages and don't use emoji.", mSpecies, mName);
	mSprite.setScale({6.2,6.2});
}

Pet::Pet(Texture texName,
	sf::IntRect texRect,
	std::string name,
	std::string species,
	std::string temper,
	std::string initPrompt,
	std::string currentStatus,
	uint32_t hunger,
	uint32_t groom,
	uint32_t health
) : mSprite(TextureManager::getInstance()->getTexture(texName), texRect),
	mTexture(texName),
	mName(name), mSpecies(species), 
	mTemper(temper), 
	mInitPrompt(initPrompt), 
	mCurrentStatus(currentStatus), 
	mHunger(hunger),
	mGroom(groom),
	mHealth(health){}

void Pet::setHungerValue(int32_t value) {

	mHunger += value;
	if (mHunger > MAX_VALUE) mHunger = MAX_VALUE;
	else if (mHunger < MIN_VALUE) mHunger = MIN_VALUE;
}
void Pet::setGroomValue(int32_t value) {

	mGroom += value;
	if (mGroom > MAX_VALUE) mGroom = MAX_VALUE;
	else if (mGroom < MIN_VALUE) mGroom = MIN_VALUE;
}
void Pet::setHealthValue(int32_t value) {

	mHealth += value;
	if (mHealth > MAX_VALUE) mHealth = MAX_VALUE;
	else if (mHealth < MIN_VALUE) mHealth = MIN_VALUE;
}

void Pet::decayValues() {

	setHungerValue(DECAY_VALUE);
	setGroomValue(DECAY_VALUE);
	if (mGroom < LIMIT_VALUE || mHunger < LIMIT_VALUE) setHealthValue(DECAY_VALUE);
}

void Pet::setSpritePosition(sf::Vector2f position){

	mSprite.setPosition(position);
}

void Pet::scaleSprite(sf::Vector2f factors) {

	mSprite.setScale(factors);
}

void Pet::toJson(nlohmann::json& j, const Pet& pet) {

	auto tex_rect = mSprite.getTextureRect();

	j = nlohmann::json{ 
		{ "name", mName },
		{ "species", mSpecies },
		{ "temper", mTemper },
		{ "initPrompt", mInitPrompt },
		{ "currentStatus", mCurrentStatus },
		{ "hunger", mHunger },
		{ "groom", mGroom },
		{ "health", mHealth },
		{"texName", mTexture},
		{ "sprite", {
			{ "position", 
				{
					{ "x", mSprite.getPosition().x },
					{ "y", mSprite.getPosition().y }
				}
			},
			{ "tex_rect", {
				{ "position", {
					{ "x", tex_rect.position.x },
					{ "y", tex_rect.position.y }
				}},
				{ "size", {
					{ "x", tex_rect.size.x },
					{ "y", tex_rect.size.y }
				}}}
			}}	
		}
	};
}

void Pet::from_json(const nlohmann::json& j, std::shared_ptr<Pet> p) {
	j.at("name").get_to(p->mName);
	j.at("species").get_to(p->mSpecies);
	j.at("temper").get_to(p->mTemper);
	j.at("initPrompt").get_to(p->mInitPrompt);
	j.at("currentStatus").get_to(p->mCurrentStatus);
	j.at("hunger").get_to(p->mHunger);
	j.at("groom").get_to(p->mGroom);
	j.at("health").get_to(p->mHealth);
	j.at("texName").get_to(p->mTexture);
	
	sf::Vector2i position = { 
		j["sprite"]["tex_rect"]["position"]["x"],
		j["sprite"]["tex_rect"]["position"]["y"] 
	};

	sf::Vector2i size = {
		j["sprite"]["tex_rect"]["size"]["x"],
		j["sprite"]["tex_rect"]["size"]["y"]
	};

	sf::IntRect tex_rect(position, size);
	p->mSprite.setTexture(TextureManager::getInstance()->getTexture(mTexture));
	p->mSprite.setTextureRect(tex_rect);
}

void Pet::setState(nlohmann::json) {

	toJson(mState, *this);
	mSaveComponent->setState(mState);
}

nlohmann::json Pet::saveData() {

	setState(mState);
	return mState;
}