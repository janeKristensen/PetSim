#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include "pet.h"
#include "model.h"
#include <queue>
#include <future>

class Game;

static void pushRequestToModel(std::string query, std::shared_ptr<Model> model) { model->userQuery(query); }

class Scene {
public:
	Scene(std::future<std::shared_ptr<Model>>& model, std::shared_ptr<sf::Texture> spritesheet, std::shared_ptr<Pet> currentPet, sf::Vector2f screenSize, sf::Font& font);
	~Scene();
	void render(sf::RenderWindow& window);
	void setCurrentPet(std::shared_ptr<Pet> pet) { mPet = std::move(pet); mPet->setSpritePosition(mPetPosition); }
	void update(float dt);
	void handleClick(sf::Vector2f mouseposition);
	void addToStringBuffer(const char text) { mStringBuffer += text; }
	void eraseFromStringBuffer();
	std::string getPrompt() { return mPromptText.getString(); }
	bool isInTextField() { return mInTextField; }

private:
	std::shared_ptr<sf::Texture> mSpritesheet = nullptr;
	std::shared_ptr<Pet> mPet = nullptr;
	std::shared_ptr<Model> mModel = nullptr;
	std::future<std::shared_ptr<Model>> mModelFuture;
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
	bool mInTextField = false;
	float mBlipTracker = 0.f;
	float mResponseTracker = 0.f;
	
};
