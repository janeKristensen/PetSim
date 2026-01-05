#include "Scene.h"



Scene::Scene(sf::Vector2f screenSize, std::shared_ptr<Game> game)
	: mScreenSize(screenSize), mGame(game) {

	std::shared_ptr<sf::RectangleShape> border = std::make_shared<sf::RectangleShape>(screenSize);
	border->setFillColor(sf::Color::Black);
	addSceneObject(SceneObject::BORDER, border);

	std::shared_ptr<sf::RectangleShape> background = std::make_shared<sf::RectangleShape>(sf::Vector2f{
			mScreenSize.x - (2 * SCREEN_MARGIN),
			mScreenSize.y - (2 * SCREEN_MARGIN)
		});
	background->setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });
	addSceneObject(SceneObject::BACKGROUND, background);
	
}


void Scene::addSceneObject(SceneObject object, std::shared_ptr<sf::RectangleShape> shape)
{
	mSceneObjects.emplace(object, shape);
}

void Scene::addTextObject(SceneText object, sf::Text&& text)
{
	mSceneText.emplace(object, std::move(text));
}


Scene::~Scene() {

	
}