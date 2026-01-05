#include "TitleScene.h"
#include "LoadingScene.h"

constexpr float BUTTON_MARGIN = 30.0;

TitleScene::TitleScene(sf::Vector2f screenSize, std::shared_ptr<Game> game) : Scene(screenSize, game)
{
	auto tm = TextureManager::getInstance();

	auto& bg = mSceneObjects.at(SceneObject::BACKGROUND);
	bg.setFillColor(sf::Color::Black);

	addSceneObject(SceneObject::START_MENU, sf::RectangleShape({
			mScreenSize.x / 2,
			mScreenSize.y / 2
		}));
	auto& menu = mSceneObjects.at(SceneObject::START_MENU);
	menu.setTexture(&TextureManager::getInstance()->getTexture(Texture::TITLE_MENU));
	menu.setPosition({ screenSize.x / 2 - menu.getSize().x / 2, screenSize.y / 2 - menu.getSize().y / 2 });

	auto pos = menu.getPosition();
	addSceneObject(SceneObject::START_BUTTON, sf::RectangleShape({ 200,50 }));
	auto& start_button = mSceneObjects.at(SceneObject::START_BUTTON);
	start_button.setTexture(&tm->getTexture(Texture::SPRITESHEET));
	start_button.setTextureRect({ {0,32}, {64,32} });
	start_button.setPosition({ pos.x + start_button.getSize().x/2, pos.y + 2 * start_button.getSize().y - BUTTON_MARGIN});

	addSceneObject(SceneObject::QUIT_BUTTON, sf::RectangleShape({ 200, 50 }));
	auto& quit_button = mSceneObjects.at(SceneObject::QUIT_BUTTON);
	quit_button.setTexture(&tm->getTexture(Texture::SPRITESHEET));
	quit_button.setTextureRect({ { 128,32 }, { 64,32 } });
	quit_button.setPosition({ start_button.getPosition().x, start_button.getPosition().y + start_button.getSize().y + BUTTON_MARGIN});
}

void TitleScene::update(float dt)
{

}


void TitleScene::render(sf::RenderWindow& window)
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


void TitleScene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::START_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		SceneManager::getInstance()->changeScene(std::make_shared<LoadingScene>(mScreenSize, mGame));
	}
	else if (mSceneObjects.at(SceneObject::QUIT_BUTTON).getGlobalBounds().contains(mouseposition))
	{
		// Action/event to exit game
	}
}


void TitleScene::handleTextEntry(const sf::Event& event)
{

}