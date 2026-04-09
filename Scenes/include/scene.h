#pragma once
#include "SFML/Graphics.hpp"
#include "TextureManager.h"
#include "FontManager.h"
#include "SceneManager.h"
#include "NeedsSystem.h"
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
	GAME_BG
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
};


class Scene {

public:
	Scene(sf::Vector2f screenSize, std::shared_ptr<Game> game);
	~Scene();

	virtual void render(sf::RenderWindow& window) {};
	virtual void update(float dt) {};
	virtual void handleClick(sf::Vector2f mouseposition) {}
	virtual void handleKeyPress(sf::Keyboard::Key key) {}
	virtual void handleHover(sf::Vector2f mouseposition) {};
	virtual void handleDrag(std::shared_ptr<sf::RenderWindow> window) {}
	virtual void handleTextEntry(const sf::Event& event) {}
	virtual void loadGame(const std::string& filename) {}
	virtual nlohmann::json setState() { mState["empty"] = ""; return mState; };

	void setEvent(std::optional<sf::Event> event) { mCurrentEvent = event; }
	void addSceneObject(SceneObject object, std::shared_ptr<sf::RectangleShape> shape);
	void addTextObject(SceneText object, sf::Text&& text);
	sf::Vector2f getObjectSize(SceneObject object) { return mSceneObjects[object]->getSize(); }
	sf::Vector2f getObjectPosition(SceneObject object) { return mSceneObjects[object]->getPosition(); }

protected:
	sf::Vector2f mScreenSize;
	std::optional<sf::Event> mCurrentEvent;
	std::unordered_map<SceneObject, std::shared_ptr<sf::RectangleShape>> mSceneObjects;
	std::unordered_map<SceneText, sf::Text> mSceneText;
	std::shared_ptr<Game> mGame;
	nlohmann::json mState;

private:

	
	
};


