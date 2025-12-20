#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include "Item.h"
#include "Pet.h"
#include "Model.h"
#include "UserInterface.h"
#include <queue>
#include <future>
#include <iostream>
#include <chrono>

class Game;

static void pushRequestToModel(std::string query, std::shared_ptr<Model> model) { model->userQuery(query); }
constexpr float SCREEN_MARGIN = 10.f;
constexpr float INV_WIDTH = 200.0f;

class Scene {
public:
	Scene(std::shared_ptr<Pet> currentPet, sf::Vector2f screenSize, const sf::Font& font);
	~Scene();
	void setModel(std::shared_ptr<Model> model);
	void render(sf::RenderWindow& window);
	void update(float dt);
	void handleClick(sf::Vector2f mouseposition);
	void handleTextEntry(const sf::Event& event);
	sf::Vector2f getInvSize() { return mInventory.getSize(); }
	sf::Vector2f getInvPosition() { return mInventory.getPosition(); }

private:
	std::string getPrompt() { return mPromptText.getString(); }
	void setCurrentPet(std::shared_ptr<Pet> pet) { mPet = pet; mPet->setSpritePosition(mPetPosition); }
	void addToStringBuffer(const char text) { mStringBuffer += text; }
	void eraseFromStringBuffer();
	bool isInTextField() { return mInTextField; }

	std::shared_ptr<Pet> mPet = nullptr;
	std::shared_ptr<Model> mModel = nullptr;
	std::shared_future<std::shared_ptr<Model>> mModelFuture;
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
	sf::Text mPetText;
#ifndef NDEBUG
	sf::Text mHealthText;
	sf::Text mHungerText;
	sf::Text mGroomText;
	sf::Text mStateText;
#endif

	ProgressBar mFoodBar = ProgressBar(sf::Vector2f{ 200, 10 }, sf::Vector2f{ SCREEN_MARGIN, SCREEN_MARGIN }, sf::Color::Red);
	ProgressBar mGroomBar = ProgressBar(sf::Vector2f{ 200, 10 }, sf::Vector2f{ SCREEN_MARGIN, 2* SCREEN_MARGIN + 2}, sf::Color::Blue);
	const sf::Font& mFont;
	bool mInTextField = false;
	float mBlipTracker = 0.f;
	float mResponseTracker = 0.f;
	
};
