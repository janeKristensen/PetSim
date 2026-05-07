#pragma once
#include "scene.h"
#include "GameScene.h"

class DialogOptions;

class WorkScene : public Scene
{
public:
	WorkScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, Services& services, GameScene& scene);
	~WorkScene() {}

	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
	void handleClick(sf::Vector2f mouseposition) override;
	void handleKeyPress(sf::Keyboard::Key key) override;
	void handleDrag(std::shared_ptr<sf::RenderWindow> window) override;
private:
	void createNumbers(size_t amount);
	sf::Vector2f getRandomPosition();

	GameScene& mGameScene;
	std::vector<std::unique_ptr<Item>> mItems;
	std::shared_ptr<Entity> mRobot = nullptr;
	sf::Vector2f mScreenPosition;
	sf::Vector2f mScreenSize;
	std::vector<std::shared_ptr<DialogOption>> mDialogOptions;
};
