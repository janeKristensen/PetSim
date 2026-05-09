#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Services.h"
#include "UserInterface.h"
#include "SceneManager.h"
#include "item.h"
#include "InventorySystem.h"
#include <functional>
#include <queue>
#include <future>
#include <iostream>
#include <optional>

class Game;

constexpr float SCREEN_MARGIN = 10.f;
constexpr float TEXT_MARGIN = 20.f;
constexpr float INV_WIDTH = 300.0f;

enum class SceneObject
{
	BORDER,
	BACKGROUND,
	START_MENU,
	START_BUTTON,
	LOAD_BUTTON,
	RETURN_BUTTON,
	SAVE_BUTTON,
	QUIT_BUTTON,
	ADD_BUTTON,
	SHOP_BUTTON,
	BUY_BUTTON,
	INVENTORY,
	TEXT_INPUT,
	TEXT_OUTPUT,
	TEXT_BLIP,
	FOOD_BAR,
	GROOM_BAR,
	HEART_ICON,
	LITTER_BOX,
	LITTER_BUTTON,
	GAME_BG,
	MONEY,
	COMPUTER, 
	DIALOG,
	OPTION_BUTTON_1,
	OPTION_BUTTON_2
};

enum class SceneText
{
	TITLE,
	PROMPT_TEXT,
	PET_TEXT,
	HEALTH_VALUE,
	HUNGER_VALUE,
	GROOM_VALUE,
	STATE_VALUE,
	MONEY_VALUE,
	HAPPY_VALUE,
	DIALOG,
	OPTION_1_TEXT,
	OPTION_2_TEXT
};


class Scene {

public:
	Scene(sf::Vector2f screenSize, std::shared_ptr<Game> game, Services& services);
	~Scene();

	virtual void render(sf::RenderWindow& window) {}
	virtual void update(float dt) {}
	virtual void handleClick(sf::Vector2f mouseposition) {}
	virtual void handleKeyPress(sf::Keyboard::Key key) {}
	virtual void handleHover(sf::Vector2f mouseposition);
	virtual void handleDrag(std::shared_ptr<sf::RenderWindow> window) {}
	virtual void handleTextEntry(const sf::Event& event) {}
	virtual void loadGame(const std::string& filename) {}
	virtual void loadShader(const std::string& filename, std::shared_ptr<sf::Shader> shader);
	virtual void setupDialog(std::shared_ptr<Entity> entity);
	virtual void resetDialog(std::shared_ptr<Entity> entity);
	virtual nlohmann::json setState() { mState["empty"] = ""; return mState; }

	Services& getServices() { return mServices; }
	void setEvent(std::optional<sf::Event> event) { mCurrentEvent = event; }
	void addSceneObject(SceneObject object, std::shared_ptr<sf::RectangleShape> shape);
	void addTextObject(SceneText object, sf::Text&& text);
	void setDialogOptions(std::vector<std::shared_ptr<DialogOption>>* options) { mDialogOptions = options; }
	void activateDialog() { mIsDialogActive = true; }
	void deactivateDialog() { mIsDialogActive = false; }
	sf::Vector2f getObjectSize(SceneObject object) { return mSceneObjects[object]->getSize(); }
	sf::Vector2f getObjectPosition(SceneObject object) { return mSceneObjects[object]->getPosition(); }
	const std::unordered_map<SceneObject, std::shared_ptr<sf::RectangleShape>> getSceneObjects() const { return mSceneObjects; }

protected:
	sf::Vector2f mScreenSize;
	std::optional<sf::Event> mCurrentEvent;
	std::unordered_map<SceneObject, std::shared_ptr<sf::RectangleShape>> mSceneObjects;
	std::unordered_map<SceneText, sf::Text> mSceneText;
	std::vector<std::shared_ptr<DialogOption>>* mDialogOptions;
	std::shared_ptr<Game> mGame;
	nlohmann::json mState;
	std::shared_ptr<sf::Shader> mShader = std::make_shared<sf::Shader>();
	std::shared_ptr<sf::Shader> mInactiveShader = std::make_shared<sf::Shader>();
	std::shared_ptr<sf::Shader> mItemShader = std::make_shared<sf::Shader>();
	Services& mServices;
	bool mIsDialogActive = false;
private:

	
	
};


