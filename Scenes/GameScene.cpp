#include "GameScene.h"


Game_Scene::Game_Scene(std::shared_ptr<Pet> currentPet, sf::Vector2f screenSize)
	: mPet(currentPet), Scene(screenSize) {

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Adding scene UI objects

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	auto& bg = mSceneObjects.at(SceneObject::BACKGROUND);
	float bg_start_X = bg.getPosition().x;
	float bg_start_Y = bg.getPosition().y;
	float bg_X = bg.getSize().x;
	float bg_Y = bg.getSize().y;


	// Button for submitting text
	float button_width = INV_WIDTH;
	float button_height = 50.f;
	addSceneObject(SceneObject::ADD_BUTTON, sf::RectangleShape({ button_width, button_height }));
	auto& button = mSceneObjects.at(SceneObject::ADD_BUTTON);
	button.setPosition(
		{
			bg_start_X + bg_X - button_width - SCREEN_MARGIN,
			bg_start_Y + bg_Y - button_height - SCREEN_MARGIN
		}
	);
	

	// Inventory container object
	float inv_height = bg_Y - (3 * SCREEN_MARGIN) - button_height;
	addSceneObject(SceneObject::INVENTORY, sf::RectangleShape({ INV_WIDTH, inv_height }));
	auto& inventory = mSceneObjects.at(SceneObject::INVENTORY);
	inventory.setFillColor(sf::Color::Red);
	inventory.setPosition(
		{
			bg_start_X + bg_X - INV_WIDTH - SCREEN_MARGIN,
			bg_start_Y + SCREEN_MARGIN
		}
	);
	

	// Text input field
	float txt_width = bg_X - button_width - (3 * SCREEN_MARGIN);
	float txt_height = 150.f;
	addSceneObject(SceneObject::TEXT_INPUT, sf::RectangleShape({ txt_width, txt_height / 3 }));
	auto& text_field = mSceneObjects.at(SceneObject::TEXT_INPUT);
	text_field.setFillColor(sf::Color::White);
	text_field.setPosition(
		{
			bg_start_X + SCREEN_MARGIN,
			bg_start_Y + bg_Y - txt_height / 3 - SCREEN_MARGIN
		}
	);


	// Text input field blip
	addSceneObject(SceneObject::TEXT_BLIP, sf::RectangleShape({ 5,20 }));
	auto& text_blip = mSceneObjects.at(SceneObject::TEXT_BLIP);
	text_blip.setFillColor(sf::Color::Black);
	text_blip.setPosition(
		{
			text_field.getPosition().x + SCREEN_MARGIN,
			text_field.getPosition().y + SCREEN_MARGIN
		}
	);


	// Place pet sprite on screen
	mPetPosition = sf::Vector2f(
		{
			bg_start_X + SCREEN_MARGIN,
			bg_start_Y + SCREEN_MARGIN
		}
	);
	mPet->setSpritePosition(mPetPosition);
	mPet->scaleSprite({5, 5});

	
	// Text output field
	addSceneObject(SceneObject::TEXT_OUTPUT, sf::RectangleShape({ txt_width, txt_height }));
	auto& output_field = mSceneObjects.at(SceneObject::TEXT_OUTPUT);
	output_field.setFillColor(sf::Color::White);
	output_field.setPosition(
		{
			bg_start_X + SCREEN_MARGIN,
			text_field.getPosition().y - txt_height - SCREEN_MARGIN
		}
	);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Adding scene text objects.

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// Output text by AI
	auto& font = FontManager::getInstance()->getFont(FontName::TITLE);
	addTextObject(SceneText::PET_TEXT, sf::Text(font, "", 24));
	auto& pet_text = mSceneText.at(SceneText::PET_TEXT);
	pet_text.setFillColor(sf::Color::Black);
	pet_text.setPosition(
		{
			output_field.getPosition().x + SCREEN_MARGIN,
			output_field.getPosition().y + SCREEN_MARGIN
		}
	);
	

	// User prompting text
	addTextObject(SceneText::PROMPT_TEXT, sf::Text(font, "", 24));
	auto& prompt_text = mSceneText.at(SceneText::PROMPT_TEXT);
	prompt_text.setFillColor(sf::Color::Black);
	prompt_text.setPosition(
		{
			text_field.getPosition().x + SCREEN_MARGIN,
			text_field.getPosition().y + SCREEN_MARGIN
		}
	);
	


#ifndef NDEBUG

	// Pet health value
	addTextObject(SceneText::HEALTH_VALUE, sf::Text(font, "", 24));
	auto& health_text = mSceneText.at(SceneText::HEALTH_VALUE);
	auto position_y = mSceneObjects.at(SceneObject::TEXT_OUTPUT).getPosition().y - SCREEN_MARGIN - health_text.getCharacterSize();
	health_text.setFillColor(sf::Color::Magenta);
	health_text.setPosition({mPetPosition.x + SCREEN_MARGIN, position_y});
	
	// Pet hunger value
	addTextObject(SceneText::HUNGER_VALUE, sf::Text(font, "", 24));
	auto& hunger_text =  mSceneText.at(SceneText::HUNGER_VALUE);
	hunger_text.setFillColor(sf::Color::Magenta);
	hunger_text.setPosition({ health_text.getPosition().x + text_field.getSize().x/4 , position_y});
	
	// Pet groom value
	addTextObject(SceneText::GROOM_VALUE, sf::Text(font, "", 24));
	auto& groom_text = mSceneText.at(SceneText::GROOM_VALUE);
	groom_text.setFillColor(sf::Color::Magenta);
	groom_text.setPosition({ hunger_text.getPosition().x + text_field.getSize().x / 2, position_y});
	
	// Pet state value
	addTextObject(SceneText::STATE_VALUE, sf::Text(font, "", 24));
	auto& state_text = mSceneText.at(SceneText::STATE_VALUE);
	state_text.setFillColor(sf::Color::Magenta);
	state_text.setPosition({ mPetPosition.x + SCREEN_MARGIN, position_y - 4*SCREEN_MARGIN});
	

#endif
}


void Game_Scene::update(float dt)
{
#ifndef NDEBUG
	mSceneText.at(SceneText::HEALTH_VALUE).setString(std::format("Health: {}", mPet->getHealthValue()));
	mSceneText.at(SceneText::HUNGER_VALUE).setString(std::format("Hunger: {}", mPet->getHungerValue()));
	mSceneText.at(SceneText::GROOM_VALUE).setString(std::format("Grooming: {}", mPet->getGroomValue()));
	mSceneText.at(SceneText::STATE_VALUE).setString(std::format("State: {}", mPet->getStatus()));
#endif
	mFoodBar.resizeBar(mPet->getHungerValue());
	mGroomBar.resizeBar(mPet->getGroomValue());


	// Update the input text field if field is active
	if (mInTextField) {
		mSceneText.at(SceneText::PROMPT_TEXT).setString(mStringBuffer);
		auto& prompt_text = mSceneText.at(SceneText::PROMPT_TEXT);
		auto& text_blip = mSceneObjects.at(SceneObject::TEXT_BLIP);
		text_blip.setPosition({prompt_text.getPosition().x + prompt_text.getGlobalBounds().size.x + 2, prompt_text.getPosition().y + 6});

		mBlipTracker += dt;

		// Blip animation
		if (mBlipTracker > 0.5) {
			text_blip.getFillColor() == sf::Color::Black ? text_blip.setFillColor(sf::Color::White) : text_blip.setFillColor(sf::Color::Black);
			mBlipTracker = 0;
		}
	}

	// if the model is loaded and theres has been no conversation for 120 seconds - auto prompt
	if (mModel != nullptr) {

		std::string str = mModel->getModelStringBuffer();
		mResponseTracker += dt;
		if (!str.empty()) {
			mSceneText.at(SceneText::PET_TEXT).setString(str);
			mResponseTracker = 0.f;
		}
		else if (mResponseTracker > 120.f) {
			mResponseTracker = 0.f;
			mFutures.push_back(std::async(std::launch::async, pushRequestToModel, "Tell me how you feel.", mModel));
		}
	}

}

void Game_Scene::render(sf::RenderWindow& window)
{
	Scene::render(window);
	window.draw(mPet->getSprite());
	window.draw(mFoodBar.getShape());
	window.draw(mGroomBar.getShape());	
}

void Game_Scene::setModel(std::shared_ptr<Model> model) 
{
	mModel = model;
}

std::string Game_Scene::getPrompt() 
{ 
	if (mSceneText.contains(SceneText::PROMPT_TEXT))
	{
		return mSceneText.at(SceneText::PROMPT_TEXT).getString();
	}
	
	return "";
}


void Game_Scene::handleClick(sf::Vector2f mouseposition) {

	mInTextField = false;

	if (mSceneObjects.at(SceneObject::ADD_BUTTON).getGlobalBounds().contains(mouseposition)) {

		if (mModel != nullptr) {

			//////////////////////////
			// TODO: is it more optimal to create a new blip every time the text field is clicked? Or create a new UI element to add a draw flag?
			/////////////////////
			mSceneObjects.at(SceneObject::TEXT_BLIP).setPosition({ -10, -10 });
			mFutures.push_back(std::async(std::launch::async, pushRequestToModel, mSceneText.at(SceneText::PROMPT_TEXT).getString(), mModel));
			mSceneText.at(SceneText::PROMPT_TEXT).setString("");
		}
	}
	else if (mSceneObjects.at(SceneObject::TEXT_INPUT).getGlobalBounds().contains(mouseposition)) {

		mStringBuffer = "";
		mInTextField = true;
	}

}

void Game_Scene::handleTextEntry(const sf::Event& event) {

	if (isInTextField()) {
		auto unicode = event.getIf<sf::Event::TextEntered>()->unicode;
		if (unicode == 8) {

			eraseFromStringBuffer();
		}
		else if (unicode < 128) {

			addToStringBuffer(static_cast<char>(unicode));
		}
	}
}

void Game_Scene::eraseFromStringBuffer() {

	if (!mStringBuffer.size() <= 0)
		mStringBuffer.pop_back();
}


Game_Scene::~Game_Scene() {

	for (auto& f : mFutures) {

		f.get();
	}
}