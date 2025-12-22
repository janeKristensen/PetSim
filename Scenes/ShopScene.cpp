#include "ShopScene.h"
#include "GameScene.h"

Shop_Scene::Shop_Scene(sf::Vector2f screenSize, Game_Scene& game) : Scene(screenSize), mGame(game)
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
}

void Shop_Scene::update(float dt)
{

}


void Shop_Scene::render(sf::RenderWindow& window)
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

	for (auto& txt : mSceneText)
	{
		window.draw(txt.second);
	}
}


void Shop_Scene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::RETURN_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		SceneManager::getInstance()->removeScene();
	}
	else if(mSceneObjects.at(SceneObject::BUY_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		// Add item to inventory
		std::shared_ptr<Item> item = std::make_shared<GroomItem>(ItemType::GROOM, Texture::SPRITESHEET, sf::IntRect({ 32,0 }, { 32,32 }), 10);
		mGame.addItemToInventory(item, 2);
	}
}


void Shop_Scene::handleTextEntry(const sf::Event& event)
{

}