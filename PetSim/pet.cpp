#include "Pet.h"
#include <iostream>

constexpr int32_t MAX_VALUE = 100;
constexpr int32_t MIN_VALUE = 0;
constexpr int32_t LIMIT_VALUE = 50;
constexpr int32_t DECAY_VALUE = -1;

Pet::Pet(std::shared_ptr<sf::Texture> spritesheet, sf::IntRect texRect, std::string name, std::string species, std::string temper) 
	: mSpritesheet(std::move(spritesheet)), mSprite(*mSpritesheet, texRect), mName(name), mSpecies(species), mTemper(temper) {

	mInitPrompt = std::format("You are a {} named {}. You are not a chat-bot but a pet. Your are {}. Answer short and cute.", mSpecies, mName, mTemper);
	mSprite.setScale({6.2,6.2});
}

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