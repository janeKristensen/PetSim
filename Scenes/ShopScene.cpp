#include "ShopScene.h"
#include "GameScene.h"
#include "PetSim.h"

ShopScene::ShopScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, Services& services, GameScene& scene) : Scene(screenSize, game, services), mGameScene(scene)
{
	std::shared_ptr<sf::RectangleShape> menu = std::make_shared<sf::RectangleShape>(sf::Vector2f{
			screenSize.x - 2 * SCREEN_MARGIN,
			screenSize.y - 2 * SCREEN_MARGIN
		});
	menu->setTexture(mServices.textureManager->getTexture(Texture::TITLE_MENU).get());
	menu->setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });
	addSceneObject(SceneObject::START_MENU, menu);


	auto pos = menu->getPosition();
	std::shared_ptr<Command> return_cmd = std::make_shared<ContinueCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> return_btn = std::make_shared<Button>(sf::Vector2f{ 32,32 }, return_cmd);
	return_btn->setTexture(mServices.textureManager->getTexture(Texture::SPRITESHEET).get());
	return_btn->setTextureRect({ {64,0}, {32,32} });
	return_btn->setPosition({ screenSize.x - SCREEN_MARGIN - return_btn->getSize().x, pos.y + SCREEN_MARGIN});
	addSceneObject(SceneObject::RETURN_BUTTON, return_btn);

	// Buy button
	std::shared_ptr<sf::RectangleShape> buy_btn = std::make_shared<Button>(sf::Vector2f{ 100,50 });
	buy_btn->setTexture(mServices.textureManager->getTexture(Texture::SPRITESHEET).get());
	buy_btn->setTextureRect({ {64,64}, {64,32} });
	buy_btn->setPosition({ screenSize.x - 2*SCREEN_MARGIN - buy_btn->getSize().x, screenSize.y - 2 * SCREEN_MARGIN - buy_btn->getSize().y });
	addSceneObject(SceneObject::BUY_BUTTON, buy_btn);
	auto buy_btn_ptr = std::static_pointer_cast<Button>(buy_btn);
	buy_btn_ptr->setInactive();
	buy_btn_ptr->setShader(mInactiveShader);


	// Add items to shop scene
	mShopItems.push_back(std::make_tuple(std::make_unique<GroomItem>(ItemType::BRUSH, Texture::SPRITESHEET, sf::IntRect({ 32,0 }, { 32,32 }), *mServices.textureManager->getTexture(Texture::SPRITESHEET), 10), 100));
	mShopItems.push_back(std::make_tuple(std::make_unique<Food>(ItemType::BONE, Texture::SPRITESHEET, sf::IntRect({ 0,0 }, { 32,32 }), *mServices.textureManager->getTexture(Texture::SPRITESHEET), 10), 100));
	mShopItems.push_back(std::make_tuple(std::make_unique<Toy>(ItemType::BALL, Texture::SPRITESHEET, sf::IntRect({ 96,0 }, { 32,32 }), *mServices.textureManager->getTexture(Texture::SPRITESHEET), 10), 200));

	sf::Vector2f start_pos = { screenSize.x / 5, screenSize.y / 3 };
	sf::Vector2f tile_size = { 200,200 };

	// Scene title text
	addTextObject(SceneText::TITLE, sf::Text(mServices.fontManager->getFont(FontName::TITLE), "Pet Shop"));
	auto& title_text = mSceneText.at(SceneText::TITLE);
	title_text.setFillColor(sf::Color::White);
	title_text.setPosition({ start_pos.x, start_pos.y - title_text.getCharacterSize() - TEXT_MARGIN});

	const auto& font = mServices.fontManager->getFont(FontName::TITLE);
	for (auto& item : mShopItems)
	{
		mShopTiles.push_back(ShopTile(tile_size, start_pos, item, font));
		start_pos.x += tile_size.x + TEXT_MARGIN;
	}
}

void ShopScene::update(float dt)
{
	for (auto& tile : mShopTiles)
	{
		tile.update();
	}

	if (mSelectedTile)
	{
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::BUY_BUTTON));
		if (mGameScene.getMoney() < mSelectedTile->getPrice())
		{
			btn->setInactive();
			btn->setShader(mInactiveShader);
		}
		else
		{
			btn->setActive();
		}
	}
}


void ShopScene::render(sf::RenderWindow& window)
{
	window.draw(*mSceneObjects.at(SceneObject::BORDER));
	window.draw(*mSceneObjects.at(SceneObject::BACKGROUND));
	window.draw(*mSceneObjects.at(SceneObject::START_MENU));

	for (auto& obj : mSceneObjects)
	{
		if (obj.first == SceneObject::BACKGROUND || 
			obj.first == SceneObject::BORDER || 
			obj.first == SceneObject::START_MENU) continue;
		
		auto btn = dynamic_pointer_cast<Button>(obj.second);
		if (btn)
		{
			if (btn->getTexture())
			{
				mShader->setUniform("texture", sf::Shader::CurrentTexture);
				mInactiveShader->setUniform("texture", sf::Shader::CurrentTexture);
				window.draw(*obj.second, btn->getShader().get());
				continue;
			}
		}
	
		window.draw(*obj.second);	
	}

	for (auto& tile : mShopTiles)
	{
		tile.render(window);
	}

	for (auto& txt : mSceneText)
	{
		window.draw(txt.second);
	}
}

void ShopScene::handleHover(sf::Vector2f mouseposition)
{
	Scene::handleHover(mouseposition);
	for (auto& tile : mShopTiles)
	{
		if (tile.getBounds().contains(mouseposition))
		{
			tile.setHoverState(true);
		}
		else
		{
			tile.setHoverState(false);
		}
	}
}

void ShopScene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::RETURN_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		mServices.soundManager->play(Sound::CLICK);
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::RETURN_BUTTON));
		btn->onClick();
	}
	else if(mSceneObjects.at(SceneObject::BUY_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		mServices.soundManager->play(Sound::CLICK);
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::BUY_BUTTON));
		if (btn->isActive())
		{
			auto cmd = btn->getCommand();
			if (!cmd)
			{
				std::shared_ptr<Command> buy_cmd = std::make_shared<BuyCommand>(shared_from_this());
				btn->setCommand(buy_cmd);
			}

			btn->onClick();
		}
	}
	else 
	{
		mSelectedTile = nullptr;
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::BUY_BUTTON));

		for (auto& tile : mShopTiles)
		{
			if (tile.getBounds().contains(mouseposition))
			{
				mServices.soundManager->play(Sound::CLICK);
				tile.selectTile(true);
				mSelectedTile = &tile;
			}
			else 
			{
				tile.selectTile(false);
			}
		}

		if (!mSelectedTile)
		{
			btn->setInactive();
			btn->setShader(mInactiveShader);
		}
	}
}


void ShopScene::handleTextEntry(const sf::Event& event)
{

}