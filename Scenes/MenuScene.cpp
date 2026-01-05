#include "MenuScene.h"
#include "UserInterface.h"

constexpr float BUTTON_MARGIN = 30.0;

MenuScene::MenuScene(sf::Vector2f screenSize, std::shared_ptr<Game> game) : Scene(screenSize, game)
{
	auto tm = TextureManager::getInstance();

	std::shared_ptr<sf::RectangleShape> menu = std::make_shared<sf::RectangleShape>(sf::Vector2f{ screenSize.x / 2, 400 });
	menu->setTexture(&tm->getTexture(Texture::TITLE_MENU));
	menu->setPosition({ screenSize.x / 2 - menu->getSize().x / 2, screenSize.y / 2 - menu->getSize().y / 2 });
	mSceneObjects.emplace(SceneObject::START_MENU, menu);

	auto pos = menu->getPosition();
	std::shared_ptr<Command> start_cmd = std::make_shared<ContinueCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> start_btn = std::make_shared<Button>(sf::Vector2f{ 200,50 }, start_cmd);
	start_btn->setTexture(&tm->getTexture(Texture::SPRITESHEET));
	start_btn->setTextureRect({ {0,64}, {64,32} });
	start_btn->setPosition({ pos.x + start_btn->getSize().x / 2, pos.y + pos.y / start_btn->getSize().y + BUTTON_MARGIN});
	mSceneObjects.emplace(SceneObject::START_BUTTON, start_btn);

	// Load game button
	std::shared_ptr<Command> load_cmd = std::make_shared<LoadCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> load_btn = std::make_shared<Button>(sf::Vector2f{ 200,50 }, load_cmd);
	load_btn->setTexture(&tm->getTexture(Texture::SPRITESHEET));
	load_btn->setTextureRect({ { 128,64 }, { 64,32 } });
	load_btn->setPosition({ start_btn->getPosition().x, start_btn->getPosition().y + start_btn->getSize().y + BUTTON_MARGIN });
	mSceneObjects.emplace(SceneObject::LOAD_BUTTON, load_btn);
	
	// Save game button
	std::shared_ptr<Command> save_cmd = std::make_shared<SaveCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> save_btn = std::make_shared<Button>(sf::Vector2f{ 200,50 }, save_cmd);
	save_btn->setPosition({ load_btn->getPosition().x, load_btn->getPosition().y + load_btn->getSize().y + BUTTON_MARGIN });
	save_btn->setTexture(&tm->getTexture(Texture::SPRITESHEET));
	save_btn->setTextureRect({ { 64,32 }, { 64,32 } });
	mSceneObjects.emplace(SceneObject::SAVE_BUTTON, save_btn);

	// Quit game button
	std::shared_ptr<Command> quit_cmd = std::make_shared<QuitCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> quit_btn = std::make_shared<Button>(sf::Vector2f{ 200,50 }, std::move(quit_cmd));
	quit_btn->setTexture(&tm->getTexture(Texture::SPRITESHEET));
	quit_btn->setTextureRect({ { 128,32 }, { 64,32 } });
	quit_btn->setPosition({ save_btn->getPosition().x, save_btn->getPosition().y + save_btn->getSize().y + BUTTON_MARGIN });
	mSceneObjects.emplace(SceneObject::QUIT_BUTTON, quit_btn);
	
}

void MenuScene::update(float dt)
{

}


void MenuScene::render(sf::RenderWindow& window)
{
	SceneManager::getInstance()->getPreviousScene()->render(window);

	for (auto& obj : mSceneObjects)
	{
		if (obj.first == SceneObject::BACKGROUND || obj.first == SceneObject::BORDER) continue;
		window.draw(*obj.second);
	}

	for (auto& txt : mSceneText)
	{
		window.draw(txt.second);
	}
}


void MenuScene::handleClick(sf::Vector2f mouseposition)
{
	auto scene_manager = SceneManager::getInstance();

	if (mSceneObjects.at(SceneObject::START_BUTTON)->getGlobalBounds().contains(mouseposition)) 
	{
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::START_BUTTON));
		btn->onClick();
	}
	else if (mSceneObjects.at(SceneObject::LOAD_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::LOAD_BUTTON));

		// How to get the filename from selection?
		static_pointer_cast<LoadCommand>(btn->getCommand())->setFilename("pretty.json");

		btn->onClick();
		scene_manager->removeScene();
	}
	else if (mSceneObjects.at(SceneObject::SAVE_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::SAVE_BUTTON));
		btn->onClick();
	}
	else if (mSceneObjects.at(SceneObject::QUIT_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::QUIT_BUTTON));
		btn->onClick();
	}
}


void MenuScene::handleTextEntry(const sf::Event& event)
{

}