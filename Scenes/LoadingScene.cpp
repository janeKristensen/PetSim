#include "LoadingScene.h"


static std::shared_ptr<Model> makeModel(std::string path, float p, float temp) {

	return std::make_shared<Model>(path, p, temp);
}

Loading_Scene::Loading_Scene(sf::Vector2f screenSize) : Scene(screenSize)
{
	/* *******************************************************************************
	*   Loading the llm model
	* ****************************************************************************/
	std::string model_path = "../llm_model/models/SmolLM2-1.7B-Instruct-IQ4_XS.gguf";
	mModelFuture = std::async(std::launch::async, makeModel, model_path, 0.2, 1.5);

	auto tm = TextureManager::getInstance();

	auto& background = mSceneObjects.at(SceneObject::BACKGROUND);
	background.setTexture(&tm->getTexture(Texture::LOADING_SCREEN));

	mSpinner = sf::CircleShape(25.f, 100);
	mSpinner.setPosition({ screenSize.x / 2, screenSize.y / 1.5f });
	mSpinner.setRotation(sf::degrees(10));
	mSpinner.setTexture(&tm->getTexture(Texture::SPRITESHEET));
	mSpinner.setTextureRect(sf::IntRect({ 126,0 }, { 32,32 }));
	mSpinner.setOrigin((sf::Vector2f)mSpinner.getLocalBounds().size / 2.f);
}

void Loading_Scene::update(float dt)
{
	mElapsedTime += dt;

	if (mElapsedTime > 5) 
	{
		// Initialize the model
		mModel = mModelFuture.get();

		if (mModel)
		{
			SceneManager::getInstance()->replaceScene(std::make_shared<Game_Scene>(mScreenSize, mModel));
		}
	}

	mSpinner.rotate(sf::degrees(180*dt));
}


void Loading_Scene::render(sf::RenderWindow& window)
{
	for (auto& obj : mSceneObjects)
	{
		window.draw(obj.second);
	}

	window.draw(mSpinner);

	for (auto& txt : mSceneText)
	{
		window.draw(txt.second);
	}

}
