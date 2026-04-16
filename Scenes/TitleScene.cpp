#include "TitleScene.h"
#include "LoadingScene.h"

constexpr float BUTTON_MARGIN = 30.0;

TitleScene::TitleScene(sf::Vector2f screenSize, std::shared_ptr<Game> game) : Scene(screenSize, game)
{
	auto tm = TextureManager::getInstance();

	auto& bg = mSceneObjects.at(SceneObject::BACKGROUND);
	bg->setFillColor(sf::Color::Black);


	std::shared_ptr<sf::RectangleShape> menu = std::make_shared<sf::RectangleShape>(sf::Vector2f{
			mScreenSize.x / 2,
			mScreenSize.y / 2
		});
	menu->setTexture(&TextureManager::getInstance()->getTexture(Texture::TITLE_MENU));
	menu->setPosition({ screenSize.x / 2 - menu->getSize().x / 2, screenSize.y / 2 - menu->getSize().y / 2 });
	addSceneObject(SceneObject::START_MENU, menu);


	auto pos = menu->getPosition();
	std::shared_ptr<Command> start_cmd = std::make_shared<StartCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> start_btn = std::make_shared<Button>(sf::Vector2f{ 200,50 }, start_cmd);
	start_btn->setTexture(&tm->getTexture(Texture::SPRITESHEET));
	start_btn->setTextureRect({ {0,32}, {64,32} });
	start_btn->setPosition({ pos.x + start_btn->getSize().x/2, pos.y + 2 * start_btn->getSize().y - BUTTON_MARGIN});
	addSceneObject(SceneObject::START_BUTTON, start_btn);


	std::shared_ptr<sf::RectangleShape> quit_btn = std::make_shared<Button>(sf::Vector2f{ 200,50 }, std::make_shared<QuitCommand>(mGame));
	quit_btn->setTexture(&tm->getTexture(Texture::SPRITESHEET));
	quit_btn->setTextureRect({ { 128,32 }, { 64,32 } });
	quit_btn->setPosition({ start_btn->getPosition().x, start_btn->getPosition().y + start_btn->getSize().y + BUTTON_MARGIN});
	addSceneObject(SceneObject::QUIT_BUTTON, quit_btn);
}

void TitleScene::update(float dt)
{

}


void TitleScene::render(sf::RenderWindow& window)
{
	for (auto& obj : mSceneObjects)
	{
		window.draw(*obj.second);
	}

	for (auto& txt : mSceneText)
	{
		window.draw(txt.second);
	}

}


void TitleScene::handleClick(sf::Vector2f mouseposition)
{
	auto sm = SoundManager::getInstance();

	if (mSceneObjects.at(SceneObject::START_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		sm->play(Sound::CLICK);
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::START_BUTTON));
		static_pointer_cast<StartCommand>(btn->getCommand())->setScreenSize(mScreenSize);
		btn->onClick();
	}
	else if (mSceneObjects.at(SceneObject::QUIT_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		sm->play(Sound::CLICK);
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::QUIT_BUTTON));
		btn->onClick();
	}
}


void TitleScene::handleTextEntry(const sf::Event& event)
{

}