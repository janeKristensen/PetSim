#include "Scene.h"



Scene::Scene(sf::Vector2f screenSize)
	: mScreenSize(screenSize) {

	addSceneObject(SceneObject::BORDER, sf::RectangleShape(screenSize));
	auto& border = mSceneObjects.at(SceneObject::BORDER);
	border.setFillColor(sf::Color::Black);
	
	addSceneObject(SceneObject::BACKGROUND, sf::RectangleShape(
		{
			mScreenSize.x - (2 * SCREEN_MARGIN),
			mScreenSize.y - (2 * SCREEN_MARGIN)
		}
	));
	auto& background = mSceneObjects.at(SceneObject::BACKGROUND);
	background.setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });
	background.setFillColor(sf::Color::Green);
}


void Scene::update(float dt) {


}

void Scene::render(sf::RenderWindow& window) {

	
}

void Scene::addSceneObject(SceneObject object, sf::RectangleShape&& shape)
{
	mSceneObjects.emplace(object, std::move(shape));
}

void Scene::addTextObject(SceneText object, sf::Text&& text)
{
	mSceneText.emplace(object, std::move(text));
}


void Scene::handleClick(sf::Vector2f mouseposition) {


}

void Scene::handleDrag(std::shared_ptr<sf::RenderWindow> window)
{

}

void Scene::handleTextEntry(const sf::Event& event) {

	
}

Scene::~Scene() {

	
}