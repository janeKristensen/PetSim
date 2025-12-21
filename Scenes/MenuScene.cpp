#include "MenuScene.h"


Menu_Scene::Menu_Scene(sf::Vector2f screenSize) : Scene(screenSize), mScreenSize(screenSize)
{
	addSceneObject(SceneObject::START_MENU, sf::RectangleShape({
			mScreenSize.x / 2,
			mScreenSize.y / 2
		}));
	auto& menu = mSceneObjects.at(SceneObject::START_MENU);
	menu.setTexture(&TextureManager::getInstance()->getTexture(Texture::TITLE_MENU));
	menu.setPosition({ screenSize.x / 2, screenSize.y / 2 });

	addSceneObject(SceneObject::START_BUTTON, sf::RectangleShape({ 300,100 }));
	auto& start_button = mSceneObjects.at(SceneObject::START_BUTTON);
	start_button.setPosition({ menu.getPosition().x / 2, menu.getPosition().y / 2 });

	addSceneObject(SceneObject::SAVE_BUTTON, sf::RectangleShape({ 300,100 }));
	auto& save_button = mSceneObjects.at(SceneObject::SAVE_BUTTON);
	save_button.setPosition({start_button.getPosition().x, start_button.getPosition().y + 2 * start_button.getSize().y});

	addSceneObject(SceneObject::QUIT_BUTTON, sf::RectangleShape({ 300,100 }));
	auto& quit_button = mSceneObjects.at(SceneObject::QUIT_BUTTON);
	quit_button.setPosition({ save_button.getPosition().x, save_button.getPosition().y + 2 * save_button.getSize().y });
}

void Menu_Scene::update(float dt)
{

}


void Menu_Scene::render(sf::RenderWindow& window)
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


void Menu_Scene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::START_BUTTON).getGlobalBounds().contains(mouseposition)) 
	{
		SceneManager::getInstance()->addScene(std::make_shared<Game_Scene>(mScreenSize));
	}
}


void Menu_Scene::handleTextEntry(const sf::Event& event)
{

}