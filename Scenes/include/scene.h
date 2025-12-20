#pragma once
#include "SFML/Graphics.hpp"
#include "FontManager.h"
#include <functional>
#include <queue>
#include <future>
#include <iostream>



constexpr float SCREEN_MARGIN = 10.f;
constexpr float INV_WIDTH = 200.0f;

enum class SceneObject
{
	BORDER,
	BACKGROUND,
	ADD_BUTTON,
	INVENTORY,
	TEXT_INPUT,
	TEXT_OUTPUT,
	TEXT_BLIP,
	FOOD_BAR,
	GROOM_BAR
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
	Scene(sf::Vector2f screenSize);
	~Scene();

	virtual void render(sf::RenderWindow& window);
	virtual void update(float dt);
	virtual void handleClick(sf::Vector2f mouseposition);
	virtual void handleTextEntry(const sf::Event& event);

	void addSceneObject(SceneObject object, sf::RectangleShape&& shape);
	void addTextObject(SceneText object, sf::Text&& text);
	sf::Vector2f getObjectSize(SceneObject object) { return mSceneObjects[object].getSize(); }
	sf::Vector2f getObjectPosition(SceneObject object) { return mSceneObjects[object].getPosition(); }

protected:
	sf::Vector2f mScreenSize;
	std::unordered_map<SceneObject, sf::RectangleShape> mSceneObjects;
	std::unordered_map<SceneText, sf::Text> mSceneText;

private:
	
};


