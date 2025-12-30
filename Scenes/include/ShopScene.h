#pragma once
#include "scene.h"
#include "UserInterface.h"

class GameScene;


class ShopScene : public Scene {
public:
	ShopScene(sf::Vector2f screenSize, GameScene& game);
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
	void handleHover(sf::Vector2f mouseposition) override;
	void handleClick(sf::Vector2f mouseposition) override;
	void handleTextEntry(const sf::Event& event) override;

private:
	GameScene& mGame;
	std::vector<std::tuple<std::unique_ptr<Item>, uint32_t>> mShopItems;
	std::vector<ShopTile> mShopTiles;
	ShopTile* mSelectedTile;
};