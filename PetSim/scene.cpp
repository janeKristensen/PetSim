#include "Scene.h"
#include <iostream>
#include <chrono>


constexpr float SCREEN_MARGIN = 10.f;
constexpr float INV_WIDTH = 200.0f;

Scene::Scene(std::future<std::shared_ptr<Model>>& model, std::shared_ptr<sf::Texture> spritesheet, std::shared_ptr<Pet> currentPet, sf::Vector2f screenSize, const sf::Font& font)
	: mFont(font), mModelFuture(model), mSpritesheet(spritesheet), mPet(currentPet), mScreenSize(screenSize), 
		mPromptText(font, "", 24), mHealthText(font, "", 24), mHungerText(font, "", 24), mGroomText(font, "", 24), mPetText(font, "", 24) {

	mBorder = sf::RectangleShape(screenSize);
	mBorder.setFillColor(sf::Color::Black);

	mBackground = sf::RectangleShape(
		{
			mScreenSize.x - (2 * SCREEN_MARGIN),
			mScreenSize.y - (2 * SCREEN_MARGIN)
		}
	);
	mBackground.setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });
	mBackground.setFillColor(sf::Color::Green);

	float bg_start_X = mBackground.getPosition().x;
	float bg_start_Y = mBackground.getPosition().y;
	float bg_X = mBackground.getSize().x;
	float bg_Y = mBackground.getSize().y;

	float button_width = INV_WIDTH;
	float button_height = 50.f;
	mButton = sf::RectangleShape({ button_width, button_height });
	mButton.setPosition(
		{
			bg_start_X + bg_X - button_width - SCREEN_MARGIN,
			bg_start_Y + bg_Y - button_height - SCREEN_MARGIN
		}
	);

	float inv_height = bg_Y - (3 * SCREEN_MARGIN) - button_height;
	mInventory = sf::RectangleShape({ INV_WIDTH, inv_height });
	mInventory.setFillColor(sf::Color::Red);
	mInventory.setPosition(
		{
			bg_start_X + bg_X - INV_WIDTH - SCREEN_MARGIN,
			bg_start_Y + SCREEN_MARGIN
		}
	);
	
	
	float txt_width = bg_X - button_width - (3* SCREEN_MARGIN);
	float txt_height = 150.f;
	mTextField = sf::RectangleShape({ txt_width, txt_height/3 });
	mTextField.setFillColor(sf::Color::White);
	mTextField.setPosition(
		{
			bg_start_X + SCREEN_MARGIN,
			bg_start_Y + bg_Y - txt_height/3 - SCREEN_MARGIN
		}
	);

	mPetTextField = sf::RectangleShape({ txt_width, txt_height });
	mPetTextField.setFillColor(sf::Color::White);
	mPetTextField.setPosition(
		{
			bg_start_X + SCREEN_MARGIN,
			mTextField.getPosition().y - txt_height - SCREEN_MARGIN
		}
	);


	mPetText.setFillColor(sf::Color::Black);
	mPetText.setPosition(
		{
			mPetTextField.getPosition().x + SCREEN_MARGIN,
			mPetTextField.getPosition().y + SCREEN_MARGIN
		}
	);


	mPromptText.setFillColor(sf::Color::Black);
	mPromptText.setPosition(
		{
			mTextField.getPosition().x + SCREEN_MARGIN,
			mTextField.getPosition().y + SCREEN_MARGIN
		}
	);

	mTextBlip = sf::RectangleShape({ 5,20 });
	mTextBlip.setFillColor(sf::Color::Black);
	mTextBlip.setPosition(mPromptText.getPosition());

	mPetPosition = sf::Vector2f(
		{
			bg_start_X + SCREEN_MARGIN,
			bg_start_Y + SCREEN_MARGIN
		}
	);

	mHealthText.setFillColor(sf::Color::Magenta);
	mHealthText.setPosition(
		{
			mPetPosition.x,
			mPetTextField.getPosition().y - SCREEN_MARGIN - mGroomText.getCharacterSize()
		}
	);

	mHungerText.setFillColor(sf::Color::Magenta);
	mHungerText.setPosition(
		{
			mPetPosition.x + (mScreenSize.x / 4),
			mPetTextField.getPosition().y - SCREEN_MARGIN - mGroomText.getCharacterSize()

		}
	);

	mGroomText.setFillColor(sf::Color::Magenta);
	mGroomText.setPosition(
		{
			mPetPosition.x + (mScreenSize.x / 4) * 2,
			mPetTextField.getPosition().y - SCREEN_MARGIN - mGroomText.getCharacterSize()
		}
	);
	
}

void Scene::update(float dt) {

	mHealthText.setString(std::format("Health: {}", mPet->getHealthValue()));
	mHungerText.setString(std::format("Hunger: {}", mPet->getHungerValue()));
	mGroomText.setString(std::format("Grooming: {}", mPet->getGroomValue()));
	
	if (mInTextField) { 
		mPromptText.setString(mStringBuffer); 
		mTextBlip.setPosition({ mPromptText.getPosition().x + mPromptText.getGlobalBounds().size.x + 2, mPromptText.getPosition().y + 6});

		mBlipTracker += dt;

		if (mBlipTracker > 0.5) {
			mTextBlip.getFillColor() == sf::Color::Black ? mTextBlip.setFillColor(sf::Color::White) : mTextBlip.setFillColor(sf::Color::Black);
			mBlipTracker = 0;
		}
	}
	
		
	if (mModel != nullptr) {

		std::string str = mModel->getModelStringBuffer();
		mResponseTracker += dt;
		if (!str.empty()) {
			mPetText.setString(str);
			mResponseTracker = 0.f;
		}

		if (mResponseTracker > 120.f) {
			mResponseTracker = 0.f;
			mFutures.push_back(std::async(std::launch::async, pushRequestToModel, "Tell me how you feel.", mModel));
		}
	}
	
}

void Scene::render(sf::RenderWindow& window) {

	window.draw(mBorder);
	window.draw(mBackground);
	window.draw(mButton);
	window.draw(mInventory);
	window.draw(mPet->getSprite());
	window.draw(mTextField);
	window.draw(mPetTextField);
	window.draw(mTextBlip);
	window.draw(mPetText);
	window.draw(mPromptText);
	window.draw(mHealthText);
	window.draw(mHungerText);
	window.draw(mGroomText);
	
	
}

void Scene::handleClick(sf::Vector2f mouseposition) {

	mInTextField = false;

	if (mButton.getGlobalBounds().contains(mouseposition)) {

		if (mModel == nullptr) {

			mModel = mModelFuture.get();
			mModel->addSystemPrompt(mPet->getInitPrompt());
		}
		
		mTextBlip.setPosition({ -10, -10 });
		std::string text = mPromptText.getString();
		mFutures.push_back(std::async(std::launch::async, pushRequestToModel, text, mModel));

		mPromptText.setString("");
		
	}
	else if (mTextField.getGlobalBounds().contains(mouseposition)) {

		mStringBuffer = "";
		mInTextField = true;
	}
	
}

void Scene::eraseFromStringBuffer() { 
	
	if(!mStringBuffer.size() <= 0)
		mStringBuffer.pop_back(); 
}


Scene::~Scene() {

	for (auto& f : mFutures) {

		f.get();
	}
	mFutures.clear();
}