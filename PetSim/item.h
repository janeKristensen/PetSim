#pragma once
#include <cstdint>
#include "SFML/Graphics.hpp"
#include <nlohmann/json.hpp>
#include "TextureManager.h"
#include <format>
#include "Memento.h"

enum class ItemType {
	BONE,
	BRUSH,
	BALL,
	POOP,
	RAKE,
	SHOVEL,
	NUMBER
};

class Item {
public:
	Item(ItemType typeId, uint32_t value, Texture texName, sf::IntRect texRect, sf::Texture& texture) 
		: mTypeId(typeId), mValue(value), mTexture(texName), mSprite(texture, texRect) {}

	Item(const ItemType typeId, const uint32_t value, const sf::Sprite& sprite, const Texture texName, const bool isAlive, const nlohmann::json& state)
		: mTypeId(typeId), mValue(value), mSprite(sprite), mTexture(texName), mIsAlive(isAlive), mState(state) {}

	Item(const Item& other);

	bool operator==(const Item& other) const { return mTypeId == other.getTypeId(); }
	bool operator==(const std::nullptr_t) const { return this == nullptr; }
	bool operator!=(const Item& other) const { return mTypeId != other.getTypeId(); }
	
	virtual std::string getDescription() { return "This is an item."; }
	virtual void printStats() const {}
	void setPosition(sf::Vector2f position) { mSprite.setPosition(position); }
	void setScale(sf::Vector2f scale) { mSprite.setScale(scale); }
	void setAlive(bool value) { mIsAlive = value; }
	void setPicked(bool value) { mIsPicked = value; }
	const bool isAlive() const { return mIsAlive; }
	const bool isPicked() const { return mIsPicked; }
	const ItemType getTypeId() const { return mTypeId; }
	const Texture getTextureName() const { return mTexture; }
	const sf::IntRect getTextureRect() const { return mSprite.getTextureRect(); }
	const sf::Vector2f getScale() const { return mSprite.getScale(); }
	void setShader(std::shared_ptr<sf::Shader> shader) { mShader = shader; }
	std::shared_ptr<sf::Shader> getShader() { return mShader; }
	
	sf::Sprite& getSprite() { return mSprite; }
	const uint32_t getValue() const { return mValue; };
	nlohmann::json saveData();

protected:
	uint32_t mValue;
	const ItemType mTypeId;
	bool mIsAlive = true;
	bool mIsPicked = false;
	sf::Sprite mSprite;
	Texture mTexture;
	nlohmann::json mState;

private:
	void setState(nlohmann::json);
	void toJson(nlohmann::json& j);

	std::shared_ptr<sf::Shader> mShader = nullptr;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>();
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
};

class Food : public Item {
public:
	Food(ItemType typeId, Texture texName, sf::IntRect texRect, sf::Texture& texture, uint32_t value) : Item(typeId, value, texName, texRect, texture) {}
	Food(const Food& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}
	
	std::string getDescription() override{ return std::format("+{} food", mValue); }
	

private:
	
};

class GroomItem : public Item {
public:
	GroomItem(ItemType typeId, Texture texName, sf::IntRect texRect, sf::Texture& texture, uint32_t value) : Item(typeId, value, texName, texRect, texture) {}
	GroomItem(const GroomItem& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}

	std::string getDescription() override { return std::format("+{} grooming", mValue); }
	

private:
	
};

class Toy : public Item {
public:
	Toy(ItemType typeId, Texture texName, sf::IntRect texRect, sf::Texture& texture, uint32_t value) : Item(typeId, value, texName, texRect, texture) {}
	Toy(const Toy& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}

	std::string getDescription() override { return std::format("+{} happiness", mValue); }
	

private:

};

class Poop : public Item {
public:
	Poop(ItemType typeId, Texture texName, sf::IntRect texRect, sf::Texture& texture, uint32_t value) : Item(typeId, value, texName, texRect, texture) {}
	Poop(const Poop& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}

	std::string getDescription() override { return std::format("-{} happiness", mValue); }


private:

};


class Rake : public Item {
public:
	Rake(ItemType typeId, Texture texName, sf::IntRect texRect, sf::Texture& texture, uint32_t value) : Item(typeId, value, texName, texRect, texture) {}
	Rake(const Rake& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}

	std::string getDescription() override { return "Used for raking litter"; }


private:

};

class Shovel : public Item {
public:
	Shovel(ItemType typeId, Texture texName, sf::IntRect texRect, sf::Texture& texture, uint32_t value) : Item(typeId, value, texName, texRect, texture) {}
	Shovel(const Shovel& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}

	std::string getDescription() override { return "Used shoveling litter"; }


private:

};

class Number : public Item {
public:
	Number(ItemType typeId, Texture texName, sf::IntRect texRect, sf::Texture& texture, uint32_t value) : Item(typeId, value, texName, texRect, texture) {}
	Number(const Number& other) : Item(other.mTypeId, other.mValue, other.mSprite, other.mTexture, other.mIsAlive, other.mState) {}

	std::string getDescription() override { return "Floating number"; }
	void setTarget(sf::Vector2f target) { mTarget = target; }
	sf::Vector2f getTarget() { return mTarget; }

private:
	sf::Vector2f mTarget;
};