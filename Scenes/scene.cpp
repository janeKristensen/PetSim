#include "Scene.h"



Scene::Scene(sf::Vector2f screenSize, std::shared_ptr<Game> game, Services& services)
	: mScreenSize(screenSize), mGame(game), mServices(services) {

	mSceneObjects.reserve(1000);
	loadShader("Shaders/ShaderBtn.frag", mShader);
	loadShader("Shaders/ShaderInactiveBtn.frag", mInactiveShader);
	loadShader("Shaders/ShaderItem.frag", mItemShader);

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

void Scene::loadShader(const std::string& filename, std::shared_ptr<sf::Shader> shader)
{
	if (!shader->isAvailable())
	{
		std::cout << "Shaders not available" << std::endl;
		throw;
	}

	if (!shader->loadFromFile(filename, sf::Shader::Type::Fragment))
	{
		std::cout << "Fragment shader file not found" << filename << std::endl;
	}
}

void Scene::addSceneObject(SceneObject object, std::shared_ptr<sf::RectangleShape> shape)
{
	mSceneObjects.emplace(object, shape);
}

void Scene::addTextObject(SceneText object, sf::Text&& text)
{
	mSceneText.emplace(object, std::move(text));
}

void Scene::handleHover(sf::Vector2f mouseposition)
{
	for (auto& obj : mSceneObjects)
	{
		auto btn = dynamic_pointer_cast<Button>(obj.second);
		if (btn)
		{
			if (obj.second->getGlobalBounds().contains(mouseposition))
			{
				if (!btn->getShader())
				{
					btn->setShader(mShader);
				}
			}
			else
			{
				if (!btn->isActive()) btn->setShader(mInactiveShader);
				else btn->setShader(nullptr);
			}
		}
	}
}

void Scene::setupDialog(std::shared_ptr<Entity> entity)
{
	if (mDialogOptions)
	{
		mSceneText.at(SceneText::DIALOG).setString(mServices.dialogManager->getDialogText(entity));
		mSceneText.at(SceneText::OPTION_1_TEXT).setString(mDialogOptions->at(0)->optionText);
		mSceneText.at(SceneText::OPTION_2_TEXT).setString(mDialogOptions->at(1)->optionText);
	}
}

void Scene::resetDialog(std::shared_ptr<Entity> entity)
{
	if (mDialogOptions)
	{
		mSceneText.at(SceneText::DIALOG).setString("");
		mSceneText.at(SceneText::OPTION_1_TEXT).setString("");
		mSceneText.at(SceneText::OPTION_2_TEXT).setString("");
	}
}


Scene::~Scene() {

	
}