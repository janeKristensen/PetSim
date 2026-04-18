#pragma once
#include <format>
#include "SFML/Graphics.hpp"
#include <nlohmann/json.hpp>
#include "TextureManager.h"
#include "Memento.h"
#include "Animation.h"

enum class AnimationName;

class Pet : public IAnimated{
public:
	Pet(Texture tex_name, 
		sf::IntRect texRect, 
		std::string name, 
		std::string species, 
		std::string temper,
		AnimationName animation
	);
	Pet(Texture tex_name,
		sf::IntRect texRect,
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
	);

	const std::string& getName() const { return mName; }
	const std::string& getSpecies() const { return mSpecies; }
	const std::string& getStatus() const { return mCurrentStatus; }
	const std::string& getInitPrompt() const { return mInitPrompt; }
	const uint32_t getHungerValue() const { return mHunger; }
	const uint32_t getGroomValue() const { return mGroom; }
	const uint32_t getHealthValue() const { return mHealth; }
	const uint32_t getHappinessValue() const { return mHappiness; }
	const bool isHappier() const { return mIsHappier; }
	const sf::Sprite getSprite() const { return mSprite; }
	void setScale(sf::Vector2f scale) { mSprite.setScale(scale); }
	//void setStatus(std::string prompt) { mCurrentStatus = prompt; }
	void setTexture(const sf::Texture& texture) override;
	void setTexRect(sf::IntRect rect) override;
	void setHungerValue(int32_t value);
	void setGroomValue(int32_t value);
	void setHealthValue(int32_t value);
	void setHappinessValue(int32_t value);
	void increasedHappiness(bool value);
	void setStatus(const std::string& str) { mCurrentStatus = str; }
	void decayValues();
	void setSpritePosition(sf::Vector2f position);
	void scaleSprite(sf::Vector2f factors);
	void from_json(const nlohmann::json& j, std::shared_ptr<Pet> p);
	nlohmann::json saveData();

private:
	void setState(nlohmann::json);
	void toJson(nlohmann::json& j, const Pet& pet);
	

	std::string mName = "";
	std::string mSpecies = "";
	std::string mTemper = "";
	std::string mInitPrompt = "";
	std::string mCurrentStatus = "";
	uint32_t mHunger = 100;
	uint32_t mGroom = 100;
	uint32_t mHealth = 100;
	uint32_t mHappiness = 100;
	bool mIsHappier = false;
	Texture mTexture;
	sf::Sprite mSprite;
	AnimationName mAnimation;
	nlohmann::json mState;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>();
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
};