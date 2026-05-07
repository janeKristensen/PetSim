#include "WorkScene.h"


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

	// Create robot
	mRobot = std::make_shared<Entity>(
		Texture::ROBOT,
		sf::IntRect({ 0,0 }, { 128,128 }),
		*mServices.textureManager->getTexture(Texture::ROBOT),
		AnimationName::ROBOT_IDLE
	);

	mRobot->setPosition({mScreenPosition.x, mScreenPosition.y + mScreenSize.y - mRobot->getSprite().getTextureRect().size.y});
	mServices.animationManager->attachAnimation(mRobot, mRobot->getAnimationName());
	mServices.dialogManager->attachDialog(mRobot, DialogName::ROBOT_FIRST_DIALOG);
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
}

void WorkScene::render(sf::RenderWindow& window)
{
	window.draw(*mSceneObjects.at(SceneObject::BORDER));
	window.draw(*mSceneObjects.at(SceneObject::BACKGROUND));
	window.draw(*mSceneObjects.at(SceneObject::START_MENU));

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

	for (auto& obj : mItems) {
		if (!obj) continue;

		/*mItemShader->setUniform("texture", sf::Shader::CurrentTexture);
		window.draw(obj->getSprite(), obj->getShader().get());*/
		window.draw(obj->getSprite());
	}

	window.draw(mRobot->getSprite());
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
		try {
			mServices.dialogManager->getDialog(mRobot)->performDialog();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << '\n';
		};
		
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
		auto random_number = rand() % (9 - 0) + 0;
		auto sprite_number = sf::Vector2i{ random_number * 32, 0 };
		mItems.push_back(std::make_unique<Number>(ItemType::NUMBER, Texture::NUMBERS, sf::IntRect(sprite_number, { 32,32 }), *mServices.textureManager->getTexture(Texture::NUMBERS), 10));
		auto item = dynamic_cast<Number*>(mItems.back().get());
		item->setPosition(getRandomPosition());
		item->setTarget(getRandomPosition());
	}
}

sf::Vector2f WorkScene::getRandomPosition()
{
	sf::Vector2f position;

	position.x = std::rand() % (int)((mScreenPosition.x + mScreenSize.x - 32) - mScreenPosition.x) + mScreenPosition.x;
	position.y = std::rand() % (int)((mScreenPosition.y + mScreenSize.y - 32) - mScreenPosition.y) + mScreenPosition.y;
	
	return position;
}


