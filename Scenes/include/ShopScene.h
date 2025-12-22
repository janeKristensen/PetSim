#pragma once
#include "scene.h"

class Game_Scene;


class Shop_Scene : public Scene {
public:
	Shop_Scene(sf::Vector2f screenSize, Game_Scene& game);
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
	void handleClick(sf::Vector2f mouseposition) override;
	void handleTextEntry(const sf::Event& event) override;

private:
	Game_Scene& mGame;
};