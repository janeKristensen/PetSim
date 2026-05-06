#pragma once
#include <format>
#include "SFML/Graphics.hpp"
#include <nlohmann/json.hpp>
#include "TextureManager.h"
#include "Memento.h"
#include "Animation.h"

enum class AnimationName;

class Entity : public IAnimated
{
public:
	Entity(Texture texName,
		sf::IntRect texRect,
		sf::Texture& texture,
		AnimationName animation) : mSprite(texture, texRect), mTexture(texName), mTextureRect(texRect), mAnimation(animation) {}

	void setPosition(sf::Vector2f position) { mSprite.setPosition(position); }
	void setScale(sf::Vector2f scale) { mSprite.setScale(scale); }
	void setTexture(std::shared_ptr<sf::Texture> texture) override;
	void setTexRect(sf::IntRect rect) override;

	sf::Vector2f getPosition() { return mSprite.getPosition(); }
	const sf::Sprite getSprite() const { return mSprite; }
	sf::IntRect getTexRect() { return mTextureRect; }
	Texture getTextureName() { return mTexture; }
	AnimationName getAnimationName() { return mAnimation; }

protected:
	Texture mTexture;
	sf::IntRect mTextureRect;
	sf::Sprite mSprite;
	AnimationName mAnimation;

private:
};

class Pet : public Entity{
public:
	Pet(
		Texture texName, 
		sf::IntRect texRect, 
		sf::Texture& texture,
		std::string name, 
		std::string species, 
		std::string temper,
		AnimationName animation
	);
	Pet(
		Texture texName,
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
	
	void setHungerValue(int32_t value);
	void setGroomValue(int32_t value);
	void setHealthValue(int32_t value);
	void setHappinessValue(int32_t value);
	void increasedHappiness(bool value);
	void setStatus(const std::string& str) { mCurrentStatus = str; }
	void decayValues();
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
	nlohmann::json mState;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>();
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
};