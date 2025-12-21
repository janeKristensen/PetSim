#pragma once
#include <future>
#include "Model.h"
#include "SFML/Graphics.hpp"
#include "Item.h"
#include "scene.h"
#include "TitleScene.h"
#include "Memento.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "NeedsSystem.h"
#include "InventorySystem.h"
#include <fstream>

constexpr sf::Vector2f screenSize{ 800.f, 600.f };


class Game {
public:
	Game(std::shared_ptr<sf::RenderWindow> window);
	void pollEvents();
	void update(float dt);
	void render();
	//std::shared_ptr<Scene> getScene() { return SceneManager::getInstance()->getScene(); }
	void saveGame();

private:
	void setState();
	
	nlohmann::json mState;
	std::vector<std::future<void>> mFutures;
	std::unique_ptr<SceneManager> mSceneManager;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>(); 
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
	std::shared_ptr<sf::RenderWindow> mWindow = nullptr;
	std::shared_future<std::shared_ptr<Model>> mModelFuture;
	
};
