#include "TitleScene.h"


Title_Scene::Title_Scene(sf::Vector2f screenSize) : Scene(screenSize), mScreenSize(screenSize)
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

	addSceneObject(SceneObject::QUIT_BUTTON, sf::RectangleShape({ 300,100 }));
	auto& quit_button = mSceneObjects.at(SceneObject::QUIT_BUTTON);
	quit_button.setPosition({ start_button.getPosition().x, start_button.getPosition().y + 2 * start_button.getSize().y });
}

void Title_Scene::update(float dt)
{

}


void Title_Scene::render(sf::RenderWindow& window)
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


void Title_Scene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::START_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		SceneManager::getInstance()->changeScene(std::make_shared<Loading_Scene>(mScreenSize));
	}
	else if (mSceneObjects.at(SceneObject::QUIT_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		// Action/event to exit game
	}
}


void Title_Scene::handleTextEntry(const sf::Event& event)
{

}