#pragma once
#include <format>
#include "SFML/Graphics.hpp"

class Pet {
public:
	Pet(std::shared_ptr<sf::Texture> spritesheet, sf::IntRect texRect, std::string name, std::string species, std::string temper);
	const std::string& getName() const { return mName; }
	const std::string& getSpecies() const { return mSpecies; }
	const std::string& getStatus() const { return mCurrentStatus; }
	const std::string& getInitPrompt() const { return mInitPrompt; }
	const uint32_t getHungerValue() const { return mHunger; }
	const uint32_t getGroomValue() const { return mGroom; }
	const uint32_t getHealthValue() const { return mHealth; }
	const sf::Sprite getSprite() const { return mSprite; }
	//void setStatus(std::string prompt) { mCurrentStatus = prompt; }
	void setHungerValue(int32_t value);
	void setGroomValue(int32_t value);
	void setHealthValue(int32_t value);
	void decayValues();
	void setSpritePosition(sf::Vector2f position);

private:
	std::string mName = "";
	std::string mSpecies = "";
	std::string mTemper = "";
	std::string mInitPrompt = "";
	std::string mCurrentStatus = "";
	int32_t mHunger = 100;
	int32_t mGroom = 100;
	int32_t mHealth = 100;
	std::shared_ptr<sf::Texture> mSpritesheet;
	sf::Sprite mSprite;
};