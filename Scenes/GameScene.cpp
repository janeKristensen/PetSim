#include "GameScene.h"
#include "ShopScene.h"
#include "MenuScene.h"
#include "LoadingScene.h"

GameScene::GameScene(sf::Vector2f screenSize, std::shared_ptr<Model> model)
	: Scene(screenSize), mModel(model), mScreenSize(screenSize) {

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Adding scene UI objects

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	mFoodBar = ProgressBar{ sf::Vector2f{ 200, 13.5 }, sf::Vector2f{ SCREEN_MARGIN, 2*SCREEN_MARGIN }, sf::Color::Red };
	auto foodbar_pos = mFoodBar.getShape().getPosition();
	mGroomBar = ProgressBar{ sf::Vector2f{ 200, 13.5 }, sf::Vector2f{ SCREEN_MARGIN, foodbar_pos.y + mFoodBar.getSize().y + SCREEN_MARGIN/2 }, sf::Color::Blue };

	auto& bg = mSceneObjects.at(SceneObject::BACKGROUND);
	bg.setFillColor(sf::Color::Black);
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
	
	// Shop button
	addSceneObject(SceneObject::SHOP_BUTTON, sf::RectangleShape({ 32,32 }));
	auto& shop_button = mSceneObjects.at(SceneObject::SHOP_BUTTON);
	shop_button.setPosition({foodbar_pos.x + mFoodBar.getFullSize().x + 4 * SCREEN_MARGIN, 2*SCREEN_MARGIN});
	shop_button.setFillColor(sf::Color::Blue);


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
			bg_start_X + 2*SCREEN_MARGIN,
			bg_start_Y + 2*SCREEN_MARGIN
		}
	);
	
	
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


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Adding scene systems and items

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	mPet = std::make_shared<Pet>(Texture::SPRITESHEET, sf::IntRect({ 0,128 }, { 64,64 }), "Kitty", "Cat", "Happy");
	mPet->setSpritePosition(mPetPosition);

	mNeedsSystem = std::make_unique<NeedsSystem>(mPet);
	mNeedsSystem->setModel(mModel);
	mInventorySystem = std::make_unique<InventorySystem>(getObjectSize(SceneObject::INVENTORY), getObjectPosition(SceneObject::INVENTORY), Texture::SPRITESHEET);

	auto food1 = std::make_shared<Food>(ItemType::FOOD, Texture::SPRITESHEET, sf::IntRect({ 0,0 }, { 32,32 }), 10);
	mInventorySystem->addItemToSlot({ 0, 0 }, food1);
	mItems.push_back(food1);

	auto food2 = std::make_shared<GroomItem>(ItemType::GROOM, Texture::SPRITESHEET, sf::IntRect({ 32,0 }, { 32,32 }), 10);
	mInventorySystem->addItemToSlot({ 0, 0 }, food2);
	mItems.push_back(food2);

	
}


void GameScene::update(float dt)
{
	for (auto& item : mItems)
	{
		if (!item) continue;
		if (!item->isAlive()) item = nullptr;
	}

	mItems.erase(std::remove(mItems.begin(), mItems.end(), nullptr), mItems.end());

	if (mItemsToAdd.size() != 0)
	{
		for (auto& item : mItemsToAdd)
		{
			mItems.push_back(item);
		}
		mItemsToAdd.erase(mItemsToAdd.begin(), mItemsToAdd.end());
	}
	
	mNeedsSystem->update(dt);
	mInventorySystem->update();

#ifndef NDEBUG
	mSceneText.at(SceneText::HEALTH_VALUE).setString(std::format("Health: {}", mPet->getHealthValue()));
	mSceneText.at(SceneText::HUNGER_VALUE).setString(std::format("Hunger: {}", mPet->getHungerValue()));
	mSceneText.at(SceneText::GROOM_VALUE).setString(std::format("Grooming: {}", mPet->getGroomValue()));
	mSceneText.at(SceneText::STATE_VALUE).setString(std::format("State: {}", mPet->getStatus()));
#endif
	mFoodBar.resizeBar((float)mPet->getHungerValue());
	mGroomBar.resizeBar((float)mPet->getGroomValue());


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

void GameScene::render(sf::RenderWindow& window)
{
	window.draw(mSceneObjects.at(SceneObject::BORDER));
	window.draw(mSceneObjects.at(SceneObject::BACKGROUND));
	window.draw(mPet->getSprite());

	for (auto& obj : mSceneObjects)
	{
		if (obj.first == SceneObject::BACKGROUND || obj.first == SceneObject::BORDER) continue;
		window.draw(obj.second);
	}
	mInventorySystem->render(window);
	for (auto obj : mItems) {
		if (!obj) continue;
#ifndef NDEBUG
		//std::cout << obj->getSprite().getPosition().x << std::endl;
#endif
		window.draw(obj->getSprite());
	}
	window.draw(mFoodBar.getShape());
	window.draw(mGroomBar.getShape());

	for (auto& txt : mSceneText)
	{
		window.draw(txt.second);
	}	

	
}

void GameScene::setModel(std::shared_ptr<Model> model) 
{
	mModel = model;
}

std::string GameScene::getPrompt() 
{ 
	if (mSceneText.contains(SceneText::PROMPT_TEXT))
	{
		return mSceneText.at(SceneText::PROMPT_TEXT).getString();
	}
	
	return "";
}


void GameScene::handleClick(sf::Vector2f mouseposition) {

	mInTextField = false;

	if (mSceneObjects.at(SceneObject::ADD_BUTTON).getGlobalBounds().contains(mouseposition)) 
	{
		if (mModel != nullptr) 
		{
			//////////////////////////
			// TODO: is it more optimal to create a new blip every time the text field is clicked? Or create a new UI element and add a draw flag?
			/////////////////////
			mSceneObjects.at(SceneObject::TEXT_BLIP).setPosition({ -10, -10 });
			mFutures.push_back(std::async(std::launch::async, pushRequestToModel, mSceneText.at(SceneText::PROMPT_TEXT).getString(), mModel));
			mSceneText.at(SceneText::PROMPT_TEXT).setString("");
		}
	}
	else if (mSceneObjects.at(SceneObject::TEXT_INPUT).getGlobalBounds().contains(mouseposition)) 
	{
		mStringBuffer = "";
		mInTextField = true;
	}
	else if (mSceneObjects.at(SceneObject::SHOP_BUTTON).getGlobalBounds().contains(mouseposition)) 
	{
		SceneManager::getInstance()->changeScene(std::make_shared<ShopScene>(mScreenSize, *this));
	}
}

void GameScene::handleKeyPress(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::M) {

		SceneManager::getInstance()->changeScene(std::make_shared<MenuScene>(mScreenSize));
	}
}

void GameScene::handleDrag(std::shared_ptr<sf::RenderWindow> window) {

	if (mItems.empty()) return;

	auto mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));

	for (auto& item : mItems) {

		if (item == nullptr) continue;
		else if (item->getSprite().getGlobalBounds().contains(mouse_position)) {

			auto remove_item = mInventorySystem->removeFromSlot(mouse_position, *item);
			
			while (!mCurrentEvent.value().is<sf::Event::MouseButtonReleased>()) {

				mouse_position = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
				mInventorySystem->dragItem(mouse_position, *item);
				
				if (mouse_position.x <= mInventorySystem->getSlotPositionAtIndex(0).x - 5*SCREEN_MARGIN)
				{
					item->setScale({5,5});
				}
				if (item->getScale().x > 1 && mouse_position.x > mInventorySystem->getSlotPositionAtIndex(0).x - 5*SCREEN_MARGIN)
				{
					item->setScale({ 1,1 });
				}
			}

			// This needs fixing - item should not collide with mouse but should spawn immediately when dragging item from slot
			if (remove_item)
			{
				mItemsToAdd.push_back(remove_item);
			}

			if (mPet->getSprite().getGlobalBounds().contains(mouse_position)) {

				mNeedsSystem->processItem(*item);
				item.reset();
			}
			else {

				mInventorySystem->addItemToSlot(mouse_position, item);
			}

			break;
		}
	}
}

void GameScene::handleTextEntry(const sf::Event& event) {

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

void GameScene::eraseFromStringBuffer() {

	if (!mStringBuffer.size() <= 0)
		mStringBuffer.pop_back();
}

nlohmann::json GameScene::setState() {

	std::vector<nlohmann::json> items;
	for (const auto& item : mItems) {

		items.push_back(item->saveData());
	}

	mState["pet"] = mPet->saveData();
	mState["items"] = items;
	mState["inventory"] = mInventorySystem->saveData();

	return mState;
}


// Move to item manager
std::shared_ptr<Item> GameScene::createItemFromType(const ItemType type, Texture texName, sf::IntRect texRect, uint32_t value)
{
	std::shared_ptr<Item> item = nullptr;

	switch (type) {
	case ItemType::FOOD:
		item = std::make_shared<Food>(type, texName, texRect, value);
		break;
	case ItemType::GROOM:
		item = std::make_shared<GroomItem>(type, texName, texRect, value);
		break;
	}
	return item;
}


void GameScene::loadGame(const std::string& filename) {

	// load json save file
	std::ifstream ifs(filename);
	nlohmann::json j;
	ifs >> j;

	mItems.clear();
	mInventorySystem->clearSlots();

	// set items in inventory
	auto slotValues = j["inventory"]["slotValues"].get<std::array<int, MAX_SLOTS>>();

	for (const auto& element : j["items"]) {

		// Get item texture rect
		sf::IntRect texRect;
		auto arr = element["position"].get<std::array<float, 6>>();
		texRect.position.x = arr[0];
		texRect.position.y = arr[1];
		texRect.size.x = arr[2];
		texRect.size.y = arr[3];

		// create new item instance from typeId
		auto typeId = element["typeId"].get<ItemType>();
		uint32_t value = element["value"].get<uint32_t>();
		Texture texName = element["texName"].get<Texture>();
		auto item = createItemFromType(typeId, texName, texRect, value);
		mItems.push_back(item);

		item->setPosition(sf::Vector2f{ arr[4], arr[5] });
		auto slot = mInventorySystem->getSlotPosition(item->getSprite().getPosition());
		mInventorySystem->addItemToSlotIndex(std::get<1>(slot), item, slotValues[std::get<1>(slot)]);
	}

	// Load Pet status from json
	mPet->from_json(j["pet"], mPet);
	std::string initPrompt = mPet->getInitPrompt() + mPet->getStatus();
	mModel->clearModelStringBuffer();
	mModel->addSystemPrompt(initPrompt);
}

void GameScene::addItemToInventory(std::shared_ptr<Item> item, std::uint32_t amount)
{
	
	auto success = mInventorySystem->spawnInInventory(item, amount);
	if (!success)
	{
		std::cout << "No room in inventory" << std::endl;
		return;
	}

	mItems.push_back(item);

	// withdraw money for object
}

GameScene::~GameScene() {

	for (auto& f : mFutures) {

		f.get();
	}
}