#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include "Pet.h"
#include "Model.h"
#include <queue>
#include <future>

class Game;

static void pushRequestToModel(std::string query, std::shared_ptr<Model> model) { model->userQuery(query); }

class Scene {
public:
	Scene(std::future<std::shared_ptr<Model>>& model, std::shared_ptr<sf::Texture> spritesheet, std::shared_ptr<Pet> currentPet, sf::Vector2f screenSize, const sf::Font& font);
	~Scene();
	void render(sf::RenderWindow& window);
	void update(float dt);
	void handleClick(sf::Vector2f mouseposition);
	void addToStringBuffer(const char text) { mStringBuffer += text; }
	void eraseFromStringBuffer();
	bool isInTextField() { return mInTextField; }
	sf::Vector2f getInvSize() { return mInventory.getSize(); }
	sf::Vector2f getInvPosition() { return mInventory.getPosition(); }

private:
	std::string getPrompt() { return mPromptText.getString(); }
	void setCurrentPet(std::shared_ptr<Pet> pet) { mPet = std::move(pet); mPet->setSpritePosition(mPetPosition); }

	std::shared_ptr<sf::Texture> mSpritesheet = nullptr;
	std::shared_ptr<Pet> mPet = nullptr;
	std::shared_ptr<Model> mModel = nullptr;
	std::future<std::shared_ptr<Model>>& mModelFuture;
	std::vector<std::future<void>> mFutures;
	std::string mStringBuffer;
	std::string mStringLine;
	sf::Vector2f mPetPosition;
	sf::Vector2f mScreenSize;
	sf::RectangleShape mBorder;
	sf::RectangleShape mBackground;
	sf::RectangleShape mInventory;
	sf::RectangleShape mButton;
	sf::RectangleShape mPetTextField;
	sf::RectangleShape mTextField;
	sf::RectangleShape mTextBlip;
	sf::Text mPromptText;
	sf::Text mHealthText;
	sf::Text mHungerText;
	sf::Text mGroomText;
	sf::Text mPetText;
	const sf::Font& mFont;
	bool mInTextField = false;
	float mBlipTracker = 0.f;
	float mResponseTracker = 0.f;
	
};
