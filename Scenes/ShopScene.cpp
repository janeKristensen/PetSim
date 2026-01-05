#include "ShopScene.h"
#include "GameScene.h"

ShopScene::ShopScene(sf::Vector2f screenSize, GameScene& game) : Scene(screenSize), mGame(game)
{
	auto tm = TextureManager::getInstance();

	addSceneObject(SceneObject::START_MENU, sf::RectangleShape({
			screenSize.x - 2*SCREEN_MARGIN,
			screenSize.y - 2*SCREEN_MARGIN
		}));
	auto& menu = mSceneObjects.at(SceneObject::START_MENU);
	menu.setTexture(&tm->getTexture(Texture::TITLE_MENU));
	menu.setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });

	auto pos = menu.getPosition();
	addSceneObject(SceneObject::RETURN_BUTTON, sf::RectangleShape({ 32,32 }));
	auto& return_button = mSceneObjects.at(SceneObject::RETURN_BUTTON);
	return_button.setTexture(&tm->getTexture(Texture::SPRITESHEET));
	return_button.setTextureRect({ {64,0}, {32,32} });
	return_button.setPosition({ screenSize.x - SCREEN_MARGIN - return_button.getSize().x, pos.y + SCREEN_MARGIN});

	addSceneObject(SceneObject::BUY_BUTTON, sf::RectangleShape({100,50}));
	auto& buy_button = mSceneObjects.at(SceneObject::BUY_BUTTON);
	buy_button.setFillColor(sf::Color::Magenta);
	buy_button.setPosition({ screenSize.x - 2*SCREEN_MARGIN - buy_button.getSize().x, screenSize.y - 2 * SCREEN_MARGIN - buy_button.getSize().y });

	// Add items to shop scene
	mShopItems.push_back(std::make_tuple(std::make_unique<GroomItem>(ItemType::BRUSH, Texture::SPRITESHEET, sf::IntRect({ 32,0 }, { 32,32 }), 10), 100));
	mShopItems.push_back(std::make_tuple(std::make_unique<Food>(ItemType::BONE, Texture::SPRITESHEET, sf::IntRect({ 0,0 }, { 32,32 }), 10), 100));

	sf::Vector2f start_pos = { screenSize.x / 5, screenSize.y / 3 };
	sf::Vector2f tile_size = { 200,200 };

	// Scene title text
	addTextObject(SceneText::TITLE, sf::Text(FontManager::getInstance()->getFont(FontName::TITLE), "Pet Shop"));
	auto& title_text = mSceneText.at(SceneText::TITLE);
	title_text.setFillColor(sf::Color::White);
	title_text.setPosition({ start_pos.x, start_pos.y - title_text.getCharacterSize() - TEXT_MARGIN});

	const auto& font = FontManager::getInstance()->getFont(FontName::TITLE);
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
}


void ShopScene::render(sf::RenderWindow& window)
{
	window.draw(mSceneObjects.at(SceneObject::BORDER));
	window.draw(mSceneObjects.at(SceneObject::BACKGROUND));
	window.draw(mSceneObjects.at(SceneObject::START_MENU));

	for (auto& obj : mSceneObjects)
	{
		if (obj.first == SceneObject::BACKGROUND || 
			obj.first == SceneObject::BORDER || 
			obj.first == SceneObject::START_MENU) continue;
		
		window.draw(obj.second);
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
	if (mSceneObjects.at(SceneObject::RETURN_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		SceneManager::getInstance()->removeScene();
	}
	else if(mSceneObjects.at(SceneObject::BUY_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		// Add item to inventory
		auto item_template = mSelectedTile->getItem();
		auto item = mGame.createItemFromType(item_template.getTypeId(), item_template.getTextureName(), item_template.getTextureRect(), item_template.getValue());
		item->setScale({1,1});
		mGame.addItemToInventory(item, 1);
	}
	else 
	{
		mSelectedTile = nullptr;
		for (auto& tile : mShopTiles)
		{
			if (tile.getBounds().contains(mouseposition))
			{
				tile.selectTile(true);
				mSelectedTile = &tile;
			}
			else 
			{
				tile.selectTile(false);
			}
		}
	}
}


void ShopScene::handleTextEntry(const sf::Event& event)
{

}