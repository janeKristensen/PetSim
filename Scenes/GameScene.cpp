#include "GameScene.h"
#include "ShopScene.h"
#include "MenuScene.h"
#include "LoadingScene.h"
#include "LitterScene.h"

GameScene::GameScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, std::shared_ptr<Model> model)
	: Scene(screenSize, game), mModel(model), mScreenSize(screenSize) {

	auto textureManager = TextureManager::getInstance();
	auto sprite_sheet = textureManager->getTexture(Texture::SPRITESHEET);
	auto animation_sheet = textureManager->getTexture(Texture::ANIMATION_SHEET);

	auto animationManager = AnimationManager::getInstance();
	animationManager->addAnimation(AnimationName::CAT, Animation(animation_sheet, 6, sf::Vector2i{ 64, 0 }, sf::Vector2i{64, 92}, 0.5));
	animationManager->addAnimation(AnimationName::CAT_HAPPY, Animation(animation_sheet, 6, sf::Vector2i{ 64, 92 }, sf::Vector2i{ 64, 92 }, 0.5));
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Adding scene UI objects

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Button size
	float button_width = 50;
	float button_height = 50.f;


	auto& bg = mSceneObjects.at(SceneObject::BACKGROUND);
	bg->setFillColor(sf::Color::Black);
	float bg_start_X = bg->getPosition().x;
	float bg_start_Y = bg->getPosition().y;
	float bg_X = bg->getSize().x;
	float bg_Y = bg->getSize().y;

	
	// Inventory container object
	float inv_height = bg_Y;
	std::shared_ptr<sf::RectangleShape> inventory = std::make_shared<sf::RectangleShape>(sf::Vector2f{ INV_WIDTH, inv_height });
	inventory->setTexture(textureManager->getTexture(Texture::INVENTORY).get());
	auto inv_pos = sf::Vector2f{
			bg_start_X,
			bg_start_Y 
	};
	inventory->setPosition(inv_pos);
	addSceneObject(SceneObject::INVENTORY, inventory);
	
	
	// Scene for pet
	std::shared_ptr<sf::RectangleShape> scene_background = std::make_shared<sf::RectangleShape>(sf::Vector2f{ 360, 640 });
	scene_background->setTexture(textureManager->getTexture(Texture::GAME_BG).get());
	scene_background->setPosition(
		{
			inv_pos.x + INV_WIDTH + SCREEN_MARGIN,
			0 + SCREEN_MARGIN
		}
	);
	addSceneObject(SceneObject::GAME_BG, scene_background);
	auto scene_bg_pos = scene_background->getPosition();
	auto scene_bg_size = scene_background->getSize();
	auto scene_end_pos = scene_bg_pos.x + scene_bg_size.x + SCREEN_MARGIN;

	// Progress bars
	auto bar_size = sf::Vector2f{ 100, 13.5 };
	auto bar_pos_x = mScreenSize.x - bar_size.x - SCREEN_MARGIN;
	mFoodBar = ProgressBar{ bar_size, sf::Vector2f{ bar_pos_x, SCREEN_MARGIN }, sf::Color::Red };
	auto foodbar_pos = mFoodBar.getShape().getPosition();
	mGroomBar = ProgressBar{ bar_size, sf::Vector2f{ bar_pos_x, foodbar_pos.y + bar_size.y + SCREEN_MARGIN / 2 }, sf::Color::Blue };


	// Button for submitting text
	auto submit_btn_pos = sf::Vector2f{
			bg_start_X + bg_X - 2*button_width,
			bg_start_Y + bg_Y - button_height
	};
	std::shared_ptr<sf::RectangleShape> submit_btn = std::make_shared<Button>(sf::Vector2f{ 2*button_width, button_height });
	submit_btn->setTexture(sprite_sheet.get());
	submit_btn->setTextureRect({ {128,128}, {64,32} });
	submit_btn->setPosition(submit_btn_pos);
	addSceneObject(SceneObject::ADD_BUTTON, submit_btn);


	// Text input field
	float txt_width = bg_X - scene_end_pos + SCREEN_MARGIN;
	float input_txt_height = 100.f;
	std::shared_ptr<sf::RectangleShape> text_field = std::make_shared<sf::RectangleShape>(sf::Vector2f{ txt_width, input_txt_height });
	text_field->setFillColor(sf::Color::White);
	auto text_field_pos = sf::Vector2f{
			scene_end_pos,
			submit_btn_pos.y - input_txt_height - SCREEN_MARGIN
	};
	text_field->setPosition(text_field_pos);
	addSceneObject(SceneObject::TEXT_INPUT, text_field);
	

	// Text output field
	float output_txt_height = 200.f;
	std::shared_ptr<sf::RectangleShape> output_field = std::make_shared<sf::RectangleShape>(sf::Vector2f{ txt_width, output_txt_height });
	output_field->setFillColor(sf::Color::White);
	auto output_field_pos = sf::Vector2f{
			scene_end_pos,
			text_field_pos.y - output_txt_height - SCREEN_MARGIN
	};
	output_field->setPosition(output_field_pos);
	addSceneObject(SceneObject::TEXT_OUTPUT, output_field);


	// Text input field blip
	std::shared_ptr<sf::RectangleShape> text_blip = std::make_shared<sf::RectangleShape>(sf::Vector2f{ 5,20 });
	text_blip->setFillColor(sf::Color::Black);
	text_blip->setPosition(
		{
			text_field_pos.x + SCREEN_MARGIN,
			text_field_pos.y + SCREEN_MARGIN
		}
	);
	addSceneObject(SceneObject::TEXT_BLIP, text_blip);

	// Money icon
	std::shared_ptr<sf::RectangleShape> money_icon = std::make_shared<sf::RectangleShape>(sf::Vector2f{ button_width, button_height });
	auto money_icon_pos = sf::Vector2f
	{
		scene_end_pos,
		SCREEN_MARGIN
	};
	money_icon->setTexture(sprite_sheet.get());
	money_icon->setTextureRect({ {0,96}, {32,32} });
	money_icon->setPosition(money_icon_pos);
	addSceneObject(SceneObject::MONEY, money_icon);

	// Shop button
	std::shared_ptr<sf::RectangleShape> shop_btn = std::make_shared<Button>(sf::Vector2f{ button_width, button_height });
	auto shop_btn_pos = sf::Vector2f
	{
		money_icon_pos.x + button_width + SCREEN_MARGIN,
		SCREEN_MARGIN
	};
	shop_btn->setTexture(sprite_sheet.get());
	shop_btn->setTextureRect({ {64,64}, {64,32} });
	shop_btn->setPosition(shop_btn_pos);
	addSceneObject(SceneObject::SHOP_BUTTON, shop_btn);


	// Litterbox button
	std::shared_ptr<sf::RectangleShape> litter_btn = std::make_shared<Button>(sf::Vector2f{ button_width,button_height });
	litter_btn->setTexture(sprite_sheet.get());
	litter_btn->setTextureRect({ {128,96}, {64,32} });
	litter_btn->setPosition({ shop_btn_pos.x + button_width + SCREEN_MARGIN, SCREEN_MARGIN });
	addSceneObject(SceneObject::LITTER_BUTTON, litter_btn);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Place pet sprite in scene

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	mPetPosition = {
		scene_bg_pos.x + scene_bg_size.x / 3.2f,
		scene_bg_pos.y + scene_bg_size.y / 2.8f
	};
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Adding scene text objects.

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	auto char_size = 24;

	// Output text by AI
	auto& font = FontManager::getInstance()->getFont(FontName::TITLE);
	addTextObject(SceneText::PET_TEXT, sf::Text(font, "", char_size));
	auto& pet_text = mSceneText.at(SceneText::PET_TEXT);
	pet_text.setFillColor(sf::Color::Black);
	pet_text.setPosition(
		{
			output_field_pos.x + SCREEN_MARGIN,
			output_field_pos.y + SCREEN_MARGIN
		}
	);
	

	// User prompting text
	addTextObject(SceneText::PROMPT_TEXT, sf::Text(font, "", char_size));
	auto& prompt_text = mSceneText.at(SceneText::PROMPT_TEXT);
	prompt_text.setFillColor(sf::Color::Black);
	prompt_text.setPosition(
		{
			text_field_pos.x + SCREEN_MARGIN,
			text_field_pos.y + SCREEN_MARGIN
		}
	);
	
	// Money value
	size_t money_char_size = 36;
	addTextObject(SceneText::MONEY_VALUE, sf::Text(font, "", money_char_size));
	auto& money_text = mSceneText.at(SceneText::MONEY_VALUE);
	auto money_text_pos = sf::Vector2f{ 
		money_icon_pos.x,
		money_icon_pos.y + button_height - money_char_size - SCREEN_MARGIN 
	};
	money_text.setFillColor(sf::Color::White);
	money_text.setPosition(money_text_pos);

#ifndef NDEBUG
	auto text_field_size = text_field->getSize();
	
	// Pet health value
	addTextObject(SceneText::HEALTH_VALUE, sf::Text(font, "", char_size));
	auto& health_text = mSceneText.at(SceneText::HEALTH_VALUE);
	auto position_y = output_field_pos.y - SCREEN_MARGIN - char_size;
	health_text.setFillColor(sf::Color::Magenta);
	auto health_text_pos = sf::Vector2f{ scene_end_pos, position_y };
	health_text.setPosition(health_text_pos);
	
	// Pet hunger value
	addTextObject(SceneText::HUNGER_VALUE, sf::Text(font, "", char_size));
	auto& hunger_text =  mSceneText.at(SceneText::HUNGER_VALUE);
	hunger_text.setFillColor(sf::Color::Magenta);
	auto hunger_text_pos = sf::Vector2f{ health_text_pos.x + text_field_size.x / 4 + SCREEN_MARGIN, position_y };
	hunger_text.setPosition(hunger_text_pos);
	
	// Pet groom value
	addTextObject(SceneText::GROOM_VALUE, sf::Text(font, "", char_size));
	auto& groom_text = mSceneText.at(SceneText::GROOM_VALUE);
	groom_text.setFillColor(sf::Color::Magenta);
	groom_text.setPosition({ hunger_text_pos.x + text_field_size.x / 4 + SCREEN_MARGIN, position_y});
	
	// Pet state value
	addTextObject(SceneText::STATE_VALUE, sf::Text(font, "", char_size));
	auto& state_text = mSceneText.at(SceneText::STATE_VALUE);
	state_text.setFillColor(sf::Color::Magenta);
	state_text.setPosition({ scene_end_pos, position_y - 4*SCREEN_MARGIN});

	// Pet happiness value
	addTextObject(SceneText::HAPPY_VALUE, sf::Text(font, "", char_size));
	auto& happy_text = mSceneText.at(SceneText::HAPPY_VALUE);
	happy_text.setFillColor(sf::Color::Magenta);
	happy_text.setPosition({ scene_end_pos, position_y - 8 * SCREEN_MARGIN });
	
#endif


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Adding scene systems and items

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	mPet = std::make_shared<Pet>(Texture::SPRITESHEET, sf::IntRect({ 64,96 }, { 64,96 }), "Kitty", "Cat", "Happy", AnimationName::CAT);
	mPet->setSpritePosition(mPetPosition);
	mPet->setScale(sf::Vector2f(2.5,2.5));
	animationManager->attachAnimation(mPet, AnimationName::CAT);

	mNeedsSystem = std::make_unique<NeedsSystem>(mPet);
	mNeedsSystem->setModel(mModel);

	mInventorySystem = std::make_unique<InventorySystem>(sf::Vector2f{INV_WIDTH, inv_height/2}, getObjectPosition(SceneObject::INVENTORY), Texture::SPRITESHEET);

	mItemScale = 2;
	auto food1 = std::make_shared<Food>(ItemType::BONE, Texture::SPRITESHEET, sf::IntRect({ 0,0 }, { 32,32 }), 10);
	food1->setScale({mItemScale, mItemScale});
	mInventorySystem->addItemToSlot({ 0, 0 }, food1);
	mItems.push_back(food1);

	auto groom = std::make_shared<GroomItem>(ItemType::BRUSH, Texture::SPRITESHEET, sf::IntRect({ 32,0 }, { 32,32 }), 10);
	groom->setScale({ mItemScale, mItemScale });
	mInventorySystem->addItemToSlot({ 0, 0 }, groom);
	mItems.push_back(groom);

	for (int i = 0; i < 10; i++)
	{
		auto toy = std::make_shared<Toy>(ItemType::BALL, Texture::SPRITESHEET, sf::IntRect({ 96,0 }, { 32,32 }), 10);
		toy->setScale({ mItemScale, mItemScale });
		mInventorySystem->addItemToSlot({ 0, 0 }, toy);
		mItems.push_back(toy);
	}
	
}


void GameScene::update(float dt)
{
	auto animationManager = AnimationManager::getInstance();
	
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
	animationManager->update(dt);

	if (mIsHappy)
	{
		auto animation = animationManager->getAnimation(mPet);
		if (animation.getCurrentFrame() == animation.getFrameNumber() - 1)
		{
			animationManager->attachAnimation(mPet, AnimationName::CAT);
			mIsHappy = false;
		}
	}

	for (auto& item : mItems)
	{
		if (!item) continue;
		if (!item->isAlive()) item = nullptr;
	}

#ifndef NDEBUG
	mSceneText.at(SceneText::HEALTH_VALUE).setString(std::format("Health: {}", mPet->getHealthValue()));
	mSceneText.at(SceneText::HUNGER_VALUE).setString(std::format("Hunger: {}", mPet->getHungerValue()));
	mSceneText.at(SceneText::GROOM_VALUE).setString(std::format("Grooming: {}", mPet->getGroomValue()));
	mSceneText.at(SceneText::STATE_VALUE).setString(std::format("State: {}", mPet->getStatus()));
	mSceneText.at(SceneText::HAPPY_VALUE).setString(std::format("Happiness: {}", mPet->getHappinessValue()));
	
#endif
	
	mSceneText.at(SceneText::MONEY_VALUE).setString(std::format("{}", mMoney));
	mFoodBar.resizeBar((float)mPet->getHungerValue());
	auto bar_pos_x = mScreenSize.x - mFoodBar.getSize().x - SCREEN_MARGIN;
	mFoodBar.setPosition(bar_pos_x);
	mGroomBar.resizeBar((float)mPet->getGroomValue());
	bar_pos_x = mScreenSize.x - mGroomBar.getSize().x - SCREEN_MARGIN;
	mGroomBar.setPosition(bar_pos_x);

	bool happiness_update = mPet->isHappier();
	if (happiness_update)
	{
		// Change animation to happy animation
		animationManager->attachAnimation(mPet, AnimationName::CAT_HAPPY);
		SoundManager::getInstance()->play(Sound::SHORT_PURR);
		mPet->increasedHappiness(false);
		mIsHappy = true;
	}

	// Update the input text field if field is active
	if (mInTextField) {
		mSceneText.at(SceneText::PROMPT_TEXT).setString(mStringBuffer);
		auto& prompt_text = mSceneText.at(SceneText::PROMPT_TEXT);
		auto& text_blip = mSceneObjects.at(SceneObject::TEXT_BLIP);
		text_blip->setPosition({prompt_text.getPosition().x + prompt_text.getGlobalBounds().size.x + 2, prompt_text.getPosition().y + 6});

		mBlipTracker += dt;

		// Blip animation
		if (mBlipTracker > 0.5) {
			text_blip->getFillColor() == sf::Color::Black ? text_blip->setFillColor(sf::Color::White) : text_blip->setFillColor(sf::Color::Black);
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
	window.draw(*mSceneObjects.at(SceneObject::BORDER));
	window.draw(*mSceneObjects.at(SceneObject::BACKGROUND));
	

	for (auto& obj : mSceneObjects)
	{
		if (obj.first == SceneObject::BACKGROUND || obj.first == SceneObject::BORDER) continue;

		auto btn = dynamic_pointer_cast<Button>(obj.second);
		if (btn)
		{
			mShader->setUniform("texture", sf::Shader::CurrentTexture);
			window.draw(*obj.second, btn->getShader().get());
		}
		else 
		{
			window.draw(*obj.second);
		}	
	}

	window.draw(mPet->getSprite());
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
	auto sm = SoundManager::getInstance();

	if (mSceneObjects.at(SceneObject::ADD_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		sm->play(Sound::CLICK);

		if (mModel != nullptr) 
		{
			//////////////////////////
			// TODO: is it more optimal to create a new blip every time the text field is clicked? Or create a new UI element and add a draw flag?
			/////////////////////
			mSceneObjects.at(SceneObject::TEXT_BLIP)->setPosition({ -10, -10 });
			mFutures.push_back(std::async(std::launch::async, pushRequestToModel, mSceneText.at(SceneText::PROMPT_TEXT).getString(), mModel));
			mSceneText.at(SceneText::PROMPT_TEXT).setString("");
			mSceneText.at(SceneText::PET_TEXT).setString("*Thinking*");
		}
	}
	else if (mSceneObjects.at(SceneObject::TEXT_INPUT)->getGlobalBounds().contains(mouseposition))
	{
		mStringBuffer = "";
		mInTextField = true;
	}
	else if (mSceneObjects.at(SceneObject::SHOP_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		sm->play(Sound::CLICK);
		SceneManager::getInstance()->changeScene(std::make_shared<ShopScene>(mScreenSize, mGame, *this));
	}
	else if (mSceneObjects.at(SceneObject::LITTER_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		sm->play(Sound::CLICK);
		SceneManager::getInstance()->changeScene(std::make_shared<LitterScene>(mScreenSize, mGame, *this));
	}
}

void GameScene::handleKeyPress(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Key::Escape) {

		SceneManager::getInstance()->changeScene(std::make_shared<MenuScene>(mScreenSize, mGame));
	}
}

void GameScene::handleDrag(std::shared_ptr<sf::RenderWindow> window) {

	if (mItems.empty()) return;

	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f mouse_position = window->mapPixelToCoords(pixelPos);

	for (auto& item : mItems) {

		if (item == nullptr) continue;
		else if (item->getSprite().getGlobalBounds().contains(mouse_position)) {

			auto soundManager = SoundManager::getInstance();
			soundManager->play(Sound::PICKUP);

			auto remove_item = mInventorySystem->removeFromSlot(mouse_position, *item);
			sf::Vector2f scale = item->getScale();
			float adjustment = 0;
			if (scale.x > mItemScale)
			{
				auto size = item->getSprite().getTexture().getSize();
				auto scale_adjusted = size.x / scale.x;
				adjustment = scale_adjusted;
			}

			// This needs fixing - item should spawn immediately when dragging item from slot
			if (remove_item)
			{
				mItemsToAdd.push_back(remove_item);
			}

			while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

				sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
				mouse_position = window->mapPixelToCoords(pixelPos);
				mouse_position.x -= adjustment;
				mouse_position.y -= adjustment;

				mInventorySystem->dragItem(mouse_position, *item);

				auto cutover = INV_WIDTH + 5 * SCREEN_MARGIN;
				if (mouse_position.x >= cutover)
				{
					item->setScale({ 5,5 });
				}
				if (item->getScale().x > mItemScale && mouse_position.x < cutover)
				{
					item->setScale({ mItemScale,mItemScale });
				}
			}

			if (mPet->getSprite().getGlobalBounds().contains(mouse_position)) {

				mNeedsSystem->processItem(*item);
				item.reset();
			}
			else {

				if (item->getScale().x > mItemScale)
				{
					item->setScale({ mItemScale,mItemScale });
				}
				mInventorySystem->addItemToSlot(mouse_position, item);
				soundManager->play(Sound::PLACE);
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

	if (type == ItemType::BONE)
	{
		item = std::make_shared<Food>(type, texName, texRect, value);
	}
	else if (type == ItemType::BRUSH)
	{
		item = std::make_shared<GroomItem>(type, texName, texRect, value);
	}
	else if (type == ItemType::BALL)
	{
		item = std::make_shared<Toy>(type, texName, texRect, value);
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
		float scale = element["scale"].get<float>();
		auto item = createItemFromType(typeId, texName, texRect, value);
		item->setScale({ scale, scale });
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
	AnimationManager::getInstance()->attachAnimation(mPet, AnimationName::CAT);
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