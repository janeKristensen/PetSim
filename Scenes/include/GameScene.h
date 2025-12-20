#pragma once
#include "scene.h"
#include "Pet.h"
#include "Model.h"
#include "UserInterface.h"

static void pushRequestToModel(std::string query, std::shared_ptr<Model> model) { model->userQuery(query); }

class Game_Scene : public Scene {
public:
	Game_Scene(std::shared_ptr<Pet> currentPet, sf::Vector2f screenSize);
	~Game_Scene();
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
	void setModel(std::shared_ptr<Model> model);
	void handleClick(sf::Vector2f mouseposition) override;
	void handleTextEntry(const sf::Event& event) override;
	

private:
	std::string getPrompt();
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
	ProgressBar mFoodBar = ProgressBar(sf::Vector2f{ 200, 10 }, sf::Vector2f{ SCREEN_MARGIN, SCREEN_MARGIN }, sf::Color::Red);
	ProgressBar mGroomBar = ProgressBar(sf::Vector2f{ 200, 10 }, sf::Vector2f{ SCREEN_MARGIN, 2 * SCREEN_MARGIN + 2 }, sf::Color::Blue);

	bool mInTextField = false;
	float mBlipTracker = 0.f;
	float mResponseTracker = 0.f;

};
