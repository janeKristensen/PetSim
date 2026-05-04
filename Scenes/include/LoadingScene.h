#pragma once
#include "scene.h"
#include "GameScene.h"


class LoadingScene : public Scene{

public:
	LoadingScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, Services& services);
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;

private:
	std::shared_ptr<Model> mModel = nullptr;
	std::shared_future<std::shared_ptr<Model>> mModelFuture;
	float mElapsedTime = 0.f;
	sf::RectangleShape mSpinner;
};