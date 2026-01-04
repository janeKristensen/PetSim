#pragma once
#include "scene.h"
#include "GameScene.h"


class MenuScene : public Scene {
public:
	MenuScene(sf::Vector2f screenSize);
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
	void handleClick(sf::Vector2f mouseposition) override;
	void handleTextEntry(const sf::Event& event) override;

private:
	sf::Vector2f mScreenSize;
};