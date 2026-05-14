#include "Pet.h"
#include <iostream>

constexpr uint32_t MAX_VALUE = 100;
constexpr uint32_t MIN_VALUE = 0;
constexpr uint32_t LIMIT_VALUE = 50;
constexpr int32_t DECAY_VALUE = -1;

void Entity::setState(nlohmann::json)
{
	toJson(mState);
	mSaveComponent->setState(mState);
}

nlohmann::json Entity::saveData()
{
	setState(mState);
	return mState;
}

void Entity::toJson(nlohmann::json& j)
{
	auto tex_rect = mSprite.getTextureRect();

	j = nlohmann::json{
		{ "texName", mTexture},
		{"animName", mAnimation},
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

void Entity::from_json(const nlohmann::json& j)
{
	j.at("texName").get_to(mTexture);
	j.at("animName").get_to(mAnimation);

	sf::Vector2i position = {
		j["sprite"]["tex_rect"]["position"]["x"],
		j["sprite"]["tex_rect"]["position"]["y"]
	};

	sf::Vector2i size = {
		j["sprite"]["tex_rect"]["size"]["x"],
		j["sprite"]["tex_rect"]["size"]["y"]
	};

	mTextureRect = sf::IntRect(position, size);
}

/********************************************************************************************************/
/* PET */
/********************************************************************************************************/


Pet::Pet(Texture tex_name,
	sf::IntRect texRect,
	sf::Texture& texture,
	std::string name,
	std::string species,
	std::string temper,
	AnimationName animation
) : Entity(tex_name,
	texRect,
	texture,
	animation), mName(name), mSpecies(species), mTemper(temper)
{
	mInitPrompt = std::format("Pretend to be a {} named {}. You are my pet not a chatbot. Give short and cute replies to messages and don't use emoji.", mSpecies, mName);
	mSprite.setScale({6.2,6.2});
}

Pet::Pet(Texture texName,
	sf::IntRect texRect,
	sf::Texture& texture,
	std::string name,
	std::string species,
	std::string temper,
	std::string initPrompt,
	std::string currentStatus,
	uint32_t hunger,
	uint32_t groom,
	uint32_t health,
	uint32_t happiness,
	AnimationName animation
) :  Entity(texName,
	texRect,
	texture,
	animation),
	mName(name), mSpecies(species), 
	mTemper(temper), 
	mInitPrompt(initPrompt), 
	mCurrentStatus(currentStatus), 
	mHunger(hunger),
	mGroom(groom),
	mHealth(health),
	mHappiness(happiness)
	{}


void Entity::setTexture(std::shared_ptr<sf::Texture> texture)
{
	mSprite.setTexture(*texture);
}

void Entity::setTexRect(sf::IntRect rect)
{
	mSprite.setTextureRect(rect);
}

void Pet::setHungerValue(int32_t value) 
{
	auto newValue = mHunger += value;
	if (newValue > MAX_VALUE) mHunger = MAX_VALUE;
	else if (newValue < MIN_VALUE) mHunger = MIN_VALUE;
	else newValue += value;
}
void Pet::setGroomValue(int32_t value) 
{
	auto newValue = mGroom += value;
	if (newValue > MAX_VALUE) mGroom = MAX_VALUE;
	else if (newValue < MIN_VALUE) mGroom = MIN_VALUE;
	else mGroom += value;
}
void Pet::setHealthValue(int32_t value) 
{
	auto newValue = mHealth += value;
	if (newValue > MAX_VALUE) mHealth = MAX_VALUE;
	else if (newValue < MIN_VALUE) mHealth = MIN_VALUE;
	else mHealth += value;
}

void Pet::setHappinessValue(int32_t value) 
{
	auto newValue = mHappiness += value;
	if (newValue > MAX_VALUE) mHappiness = MAX_VALUE;
	else if (newValue < MIN_VALUE) mHappiness = MIN_VALUE;
	else mHappiness += value;
}

void Pet::increasedHappiness(bool value)
{
	mIsHappier = value;
}

void Pet::decayValues() 
{
	setHungerValue(DECAY_VALUE);
	setGroomValue(DECAY_VALUE);
	setHappinessValue(DECAY_VALUE);
	if (mGroom < LIMIT_VALUE || mHunger < LIMIT_VALUE) setHealthValue(DECAY_VALUE);
	if (mHealth < LIMIT_VALUE) setHappinessValue(DECAY_VALUE);
}

void Pet::toJson(nlohmann::json& j) 
{
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
		{ "happiness", mHappiness},
		{ "texName", mTexture},
		{"animName", mAnimation},
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

void Pet::from_json(const nlohmann::json& j) 
{
	j.at("name").get_to(mName);
	j.at("species").get_to(mSpecies);
	j.at("temper").get_to(mTemper);
	j.at("initPrompt").get_to(mInitPrompt);
	j.at("currentStatus").get_to(mCurrentStatus);
	j.at("hunger").get_to(mHunger);
	j.at("groom").get_to(mGroom);
	j.at("health").get_to(mHealth);
	j.at("happiness").get_to(mHappiness);
	j.at("texName").get_to(mTexture);
	j.at("animName").get_to(mAnimation);
	
	sf::Vector2i position = { 
		j["sprite"]["tex_rect"]["position"]["x"],
		j["sprite"]["tex_rect"]["position"]["y"] 
	};

	sf::Vector2i size = {
		j["sprite"]["tex_rect"]["size"]["x"],
		j["sprite"]["tex_rect"]["size"]["y"]
	};

	mTextureRect = sf::IntRect(position, size);
}

