#include "MenuScene.h"

constexpr float BUTTON_MARGIN = 30.0;

MenuScene::MenuScene(sf::Vector2f screenSize) : Scene(screenSize)
{
	auto tm = TextureManager::getInstance();

	addSceneObject(SceneObject::START_MENU, sf::RectangleShape({
			screenSize.x / 2,
			screenSize.y / 2
		}));
	auto& menu = mSceneObjects.at(SceneObject::START_MENU);
	menu.setTexture(&tm->getTexture(Texture::TITLE_MENU));
	menu.setPosition({ screenSize.x / 2 - menu.getSize().x / 2, screenSize.y / 2 - menu.getSize().y / 2 });

	auto pos = menu.getPosition();
	addSceneObject(SceneObject::START_BUTTON, sf::RectangleShape({ 200,50 }));
	auto& start_button = mSceneObjects.at(SceneObject::START_BUTTON);
	start_button.setTexture(&tm->getTexture(Texture::SPRITESHEET));
	start_button.setTextureRect({ {0,64}, {64,32} });
	start_button.setPosition({ pos.x + start_button.getSize().x / 2, pos.y + pos.y / start_button.getSize().y + BUTTON_MARGIN});

	addSceneObject(SceneObject::SAVE_BUTTON, sf::RectangleShape({ 200,50 }));
	auto& save_button = mSceneObjects.at(SceneObject::SAVE_BUTTON);
	save_button.setTexture(&tm->getTexture(Texture::SPRITESHEET));
	save_button.setTextureRect({{ 64,32 }, { 64,32 }});
	save_button.setPosition({start_button.getPosition().x, start_button.getPosition().y + start_button.getSize().y + BUTTON_MARGIN});

	addSceneObject(SceneObject::QUIT_BUTTON, sf::RectangleShape({ 200,50 }));
	auto& quit_button = mSceneObjects.at(SceneObject::QUIT_BUTTON);
	quit_button.setTexture(&tm->getTexture(Texture::SPRITESHEET));
	quit_button.setTextureRect({ { 128,32 }, { 64,32 } });
	quit_button.setPosition({ save_button.getPosition().x, save_button.getPosition().y + save_button.getSize().y + BUTTON_MARGIN });
}

void MenuScene::update(float dt)
{

}


void MenuScene::render(sf::RenderWindow& window)
{
	for (auto& obj : mSceneObjects)
	{
		window.draw(obj.second);
	}

	for (auto& txt : mSceneText)
	{
		window.draw(txt.second);
	}

}


void MenuScene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::START_BUTTON).getGlobalBounds().contains(mouseposition)) 
	{
		SceneManager::getInstance()->removeScene();
	}
	else if (mSceneObjects.at(SceneObject::SAVE_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		// Action/event to save game
	}
	else if (mSceneObjects.at(SceneObject::QUIT_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		// Action/event to exit game
	}
}


void MenuScene::handleTextEntry(const sf::Event& event)
{

}