#pragma once
#include "scene.h"
#include "GameScene.h"


class Loading_Scene : public Scene{

public:
	Loading_Scene(sf::Vector2f screenSize);
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;

private:
	std::shared_ptr<Model> mModel = nullptr;
	std::shared_future<std::shared_ptr<Model>> mModelFuture;
	float mElapsedTime = 0.f;
	sf::CircleShape mSpinner;
};