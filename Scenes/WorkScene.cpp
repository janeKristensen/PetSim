#include "WorkScene.h"

const size_t DIALOG_FONT_SIZE = 16;
const size_t ROBOT_HEIGHT = 128;

WorkScene::WorkScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, Services& services, GameScene& scene)
	: Scene(screenSize, game, services), mGameScene(scene)
{

	mItems.reserve(50);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Adding scene UI objects

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Background
	auto& bg = mSceneObjects.at(SceneObject::BACKGROUND);
	bg->setFillColor(sf::Color::Black);
	float bg_start_X = bg->getPosition().x;
	float bg_start_Y = bg->getPosition().y;
	float bg_X = bg->getSize().x;
	float bg_Y = bg->getSize().y;

	// Main screen
	std::shared_ptr<sf::RectangleShape> menu = std::make_shared<sf::RectangleShape>(sf::Vector2f{
			screenSize.x - 2 * SCREEN_MARGIN,
			screenSize.y - 2 * SCREEN_MARGIN
		});
	menu->setTexture(mServices.textureManager->getTexture(Texture::TITLE_MENU).get());
	menu->setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });
	addSceneObject(SceneObject::START_MENU, menu);
	mScreenPosition = menu->getPosition();
	mScreenSize = menu->getSize();

	// Return button
	std::shared_ptr<Command> return_cmd = std::make_shared<ContinueCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> return_btn = std::make_shared<Button>(sf::Vector2f{ 32,32 }, return_cmd);
	return_btn->setTexture(mServices.textureManager->getTexture(Texture::SPRITESHEET).get());
	return_btn->setTextureRect({ {64,0}, {32,32} });
	return_btn->setPosition({ screenSize.x - SCREEN_MARGIN - return_btn->getSize().x, mScreenPosition.y + SCREEN_MARGIN });
	addSceneObject(SceneObject::RETURN_BUTTON, return_btn);

	// Populate moving numbers
	createNumbers(20);

	// Dialogs for robot
	mRobotDialog.push(DialogName::ROBOT_FIRST_DIALOG);
	mRobotDialog.push(DialogName::ROBOT_SECOND_DIALOG);
		
	// Create robot
	sf::Vector2i robot_size = { 128,ROBOT_HEIGHT };
	mRobot = std::make_shared<Entity>(
		Texture::ROBOT,
		sf::IntRect({ 0,0 }, robot_size),
		*mServices.textureManager->getTexture(Texture::ROBOT),
		AnimationName::ROBOT_IDLE
	);
	auto robot_position = sf::Vector2f{ mScreenPosition.x, mScreenPosition.y + mScreenSize.y - robot_size.y };
	mRobot->setPosition(robot_position);
	mServices.animationManager->attachAnimation(mRobot, mRobot->getAnimationName());
	mServices.dialogManager->attachDialog(mRobot, mRobotDialog.front());


	// Dialog box
	auto dialogSize = sf::Vector2f{ 500,128 };
	sf::Vector2f btn_size = { dialogSize.x - 2*SCREEN_MARGIN, 32 };
	std::shared_ptr<sf::RectangleShape> dialog = std::make_shared<sf::RectangleShape>(dialogSize);
	//menu->setTexture(mServices.textureManager->getTexture(Texture::TITLE_MENU).get());
	dialog->setPosition({ robot_position.x + robot_size.x + SCREEN_MARGIN, robot_position.y });
	addSceneObject(SceneObject::DIALOG, dialog);
	auto dialogPos = dialog->getPosition();
	

	// Dialog options button
	mDialogOptions = *mServices.dialogManager->getDialogOptions(mRobot);
	std::shared_ptr<Command> option1 = std::make_shared<DialogCommand>(mRobot, mGame, &mDialogOptions, 0);
	std::shared_ptr<Command> option2 = std::make_shared<DialogCommand>(mRobot, mGame, &mDialogOptions, 1);
	

	// option button 1
	std::shared_ptr<sf::RectangleShape> option_btn = std::make_shared<Button>(btn_size, option1);
	option_btn->setTexture(mServices.textureManager->getTexture(Texture::CHOICE_BTN).get());
	option_btn->setTextureRect({ {0,0}, (sf::Vector2i)btn_size });
	auto optionPos1 = sf::Vector2f{dialogPos.x + SCREEN_MARGIN, dialogPos.y + SCREEN_MARGIN + 2 * DIALOG_FONT_SIZE };
	option_btn->setPosition(optionPos1);
	option_btn->setFillColor(sf::Color::White);
	addSceneObject(SceneObject::OPTION_BUTTON_1, option_btn);

	// option button 2
	std::shared_ptr<sf::RectangleShape> option_btn2 = std::make_shared<Button>(btn_size, option2);
	option_btn2->setTexture(mServices.textureManager->getTexture(Texture::CHOICE_BTN).get());
	option_btn2->setTextureRect({ {0,0}, (sf::Vector2i)btn_size });
	option_btn2->setFillColor(sf::Color::White);
	auto optionPos2 = sf::Vector2f{ dialogPos.x + SCREEN_MARGIN, optionPos1.y + option_btn->getSize().y + SCREEN_MARGIN };
	option_btn2->setPosition(optionPos2);
	addSceneObject(SceneObject::OPTION_BUTTON_2, option_btn2);

	// Option text
	auto& font = mServices.fontManager->getFont(FontName::TITLE);
	size_t text_offset = SCREEN_MARGIN * 2;
	addTextObject(SceneText::DIALOG, sf::Text(font, "", DIALOG_FONT_SIZE));
	auto& dialog_text = mSceneText.at(SceneText::DIALOG);
	dialog_text.setFillColor(sf::Color::Black);
	dialog_text.setPosition({ dialogPos.x + SCREEN_MARGIN , dialogPos.y + SCREEN_MARGIN });

	addTextObject(SceneText::OPTION_1_TEXT, sf::Text(font, "", DIALOG_FONT_SIZE));
	auto& opt1_text = mSceneText.at(SceneText::OPTION_1_TEXT);
	opt1_text.setFillColor(sf::Color::Black);
	opt1_text.setPosition({ optionPos1.x + text_offset, optionPos1.y });

	addTextObject(SceneText::OPTION_2_TEXT, sf::Text(font, "", DIALOG_FONT_SIZE));
	auto& opt2_text = mSceneText.at(SceneText::OPTION_2_TEXT);
	opt2_text.setFillColor(sf::Color::Black);
	opt2_text.setPosition({ optionPos2.x + text_offset, optionPos2.y });
}


void WorkScene::update(float dt)
{
	mItems.erase(std::remove(mItems.begin(), mItems.end(), nullptr), mItems.end());

	mServices.animationManager->update(dt);

	for (auto& obj : mItems)
	{
		auto item = dynamic_cast<Number*>(obj.get());
		if (item)
		{
			auto position = item->getSprite().getPosition();
			sf::Vector2f dir = item->getTarget() - position;
			float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

			if (length < 5.f) {
				item->setTarget(getRandomPosition());
			}
			else {
				dir /= length;
				auto scalar = 5 * dt;
				position += {dir.x * scalar, dir.y * scalar};
				item->setPosition(position);
			}
		}
	}

	for (auto& item : mItems)
	{
		if (!item) continue;
		if (!item->isAlive()) item = nullptr;
	}

	mGameScene.update(dt);
}

void WorkScene::render(sf::RenderWindow& window)
{
	window.draw(*mSceneObjects.at(SceneObject::BORDER));
	window.draw(*mSceneObjects.at(SceneObject::BACKGROUND));
	window.draw(*mSceneObjects.at(SceneObject::START_MENU));

	if(mIsDialogActive)
	{
		window.draw(*mSceneObjects.at(SceneObject::DIALOG));
	}

	for (auto& obj : mSceneObjects)
	{
		if (obj.first == SceneObject::BACKGROUND || 
			obj.first == SceneObject::BORDER ||
			obj.first == SceneObject::DIALOG ||
			obj.first == SceneObject::START_MENU) continue;
		
		auto btn = dynamic_pointer_cast<Button>(obj.second);
		if (btn)
		{
			if (obj.first == SceneObject::OPTION_BUTTON_1 ||
				obj.first == SceneObject::OPTION_BUTTON_2)
			{
				if (!mIsDialogActive)
				{
					continue;
				}
			}
			
			if (btn->getTexture())
			{
				mShader->setUniform("texture", sf::Shader::CurrentTexture);
				window.draw(*obj.second, btn->getShader().get());
				continue;
			}
		}

		window.draw(*obj.second);	
	}

	for (auto& obj : mItems) {
		if (!obj) continue;

		/*mItemShader->setUniform("texture", sf::Shader::CurrentTexture);
		window.draw(obj->getSprite(), obj->getShader().get());*/
		window.draw(obj->getSprite());
	}

	window.draw(mRobot->getSprite());

	for (auto& txt : mSceneText)
	{
		if (txt.first == SceneText::DIALOG ||
			txt.first == SceneText::OPTION_1_TEXT||
			txt.first == SceneText::OPTION_2_TEXT)
		{
			if (!mIsDialogActive)
			{
				continue;
			}
		}
		
		window.draw(txt.second);	
	}
}

void WorkScene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::RETURN_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		mServices.soundManager->play(Sound::CLICK);
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::RETURN_BUTTON));
		btn->onClick();
	}
	else if (mRobot->getSprite().getGlobalBounds().contains(mouseposition))
	{
		mServices.soundManager->play(Sound::CLICK);
		try 
		{
			if (!mServices.dialogManager->getDialog(mRobot))
			{
				if (!mRobotDialog.empty())
				{
					mServices.dialogManager->attachDialog(mRobot, mRobotDialog.front());
					startDialog(mRobot);
				}	
			}
			else
			{
				startDialog(mRobot);
			}

			if (!mRobotDialog.empty())
			{
				mRobotDialog.pop();
			}
		}
		catch (const std::exception& e) 
		{
			std::cout << e.what() << '\n';
		};
		
	}
	else if (mSceneObjects.at(SceneObject::OPTION_BUTTON_1)->getGlobalBounds().contains(mouseposition))
	{
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::OPTION_BUTTON_1));
		btn->onClick();
		setupDialog(mRobot);
	}
	else if (mSceneObjects.at(SceneObject::OPTION_BUTTON_2)->getGlobalBounds().contains(mouseposition))
	{
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::OPTION_BUTTON_2));
		btn->onClick();
		setupDialog(mRobot);
	}
	else 
	{
		for (auto& obj : mItems)
		{
			if (obj->getSprite().getGlobalBounds().contains(mouseposition))
			{
				mGameScene.addMoney(obj->getValue());
				obj->setAlive(false);
				return;
			}
		}
	}
}

void WorkScene::handleKeyPress(sf::Keyboard::Key key)
{

}

void WorkScene::handleDrag(std::shared_ptr<sf::RenderWindow> window)
{

}

void WorkScene::createNumbers(size_t amount)
{
	for (int i = 0; i < amount; i++)
	{
		auto random_number = rand() % 2;
		auto sprite_number = sf::Vector2i{ random_number * 32, 0 };
		mItems.push_back(std::make_shared<Number>(ItemType::NUMBER, Texture::NUMBERS, sf::IntRect(sprite_number, { 32,32 }), *mServices.textureManager->getTexture(Texture::NUMBERS), 10));
		auto item = dynamic_cast<Number*>(mItems.back().get());
		item->setPosition(getRandomPosition());
		item->setTarget(getRandomPosition());
	}
}

sf::Vector2f WorkScene::getRandomPosition()
{
	sf::Vector2f position;

	position.x = std::rand() % (int)((mScreenPosition.x + mScreenSize.x - 32) - mScreenPosition.x) + mScreenPosition.x;
	position.y = std::rand() % (int)((mScreenPosition.y + mScreenSize.y - ROBOT_HEIGHT-32) - mScreenPosition.y) + mScreenPosition.y;
	
	return position;
}

void WorkScene::setState()
{
	std::vector<nlohmann::json> items;
	for (const auto& item : mItems) {

		items.push_back(item->saveData());
	}

	std::vector<nlohmann::json> dialog;
	while(!mRobotDialog.empty()) {

		dialog.push_back(mRobotDialog.front());
		mRobotDialog.pop();
	}

	mState["items"] = items;
	mState["robot"] = mRobot->saveData();
	mState["dialogNames"] = dialog;
}

void WorkScene::loadData(nlohmann::json data)
{
	mRobot->loadData(data["robot"]);

	for (const auto& element : data["items"])
	{
		// create new item instance from typeId
		auto typeId = element["typeId"].get<ItemType>();
		auto item = mServices.itemManager->createItemFromType(typeId);
		item->loadData(element);
		item->setTexture(mServices.textureManager->getTexture(item->getTextureName()));
		mItems.push_back(item);
	}

	for (const auto& dialog : data["dialogNames"])
	{
		mRobotDialog.push(dialog);
	}

	mServices.dialogManager->attachDialog(mRobot, mRobotDialog.front());
}

