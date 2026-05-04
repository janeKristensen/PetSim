#pragma once
#include <future>
#include "Model.h"
#include "SFML/Graphics.hpp"
#include "Services.h"
#include "item.h"
#include "TitleScene.h"
#include "LitterScene.h"
#include "Memento.h"
#include <fstream>
#include <ctime>

//constexpr sf::Vector2f SCREEN_SIZE{ 800.f, 600.f };


class Game : public std::enable_shared_from_this<Game>{
public:
	Game(std::shared_ptr<sf::RenderWindow> window);
	void init();
	void pollEvents();
	void update(float dt);
	void render();
	//std::shared_ptr<Scene> getScene() { return SceneManager::getInstance()->getScene(); }
	void saveGame();
	void loadGame(const std::string& filename);
	void quitGame();
	Services& getServices() { return mServices; }

private:
	void setState();
	
	nlohmann::json mState;
	std::vector<std::future<void>> mFutures;
	std::shared_ptr<SaveComponent> mSaveComponent = std::make_shared<SaveComponent>(); 
	std::unique_ptr<SaveManager> mSaveManager = std::make_unique<SaveManager>(mSaveComponent);
	std::shared_ptr<sf::RenderWindow> mWindow = nullptr;
	bool mMouseDown = false;
	bool mIsDragging = false;
	sf::Vector2f mMouseDownPosition;

	NeedsSystem mNeedsSystem;
	AnimationManager mAnimationManager;
	FontManager mFontManager;
	SoundManager mSoundManager;
	TextureManager mTextureManager;

	Services mServices;
};
