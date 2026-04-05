#pragma once
#include "scene.h"
#include "UserInterface.h"

class GameScene;


class ShopScene : public Scene, public std::enable_shared_from_this<ShopScene> {
public:
	ShopScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, GameScene& scene);
	~ShopScene() { delete mSelectedTile; }
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
	void handleHover(sf::Vector2f mouseposition) override;
	void handleClick(sf::Vector2f mouseposition) override;
	void handleTextEntry(const sf::Event& event) override;
	ShopTile& getSelectedTile() const { return *mSelectedTile; }
	GameScene& getGameScene() { return mGameScene; }

private:
	std::vector<std::tuple<std::unique_ptr<Item>, uint32_t>> mShopItems;
	std::vector<ShopTile> mShopTiles;
	ShopTile* mSelectedTile = nullptr;
	GameScene& mGameScene;
};