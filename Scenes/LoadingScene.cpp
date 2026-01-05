#include "LoadingScene.h"


static std::shared_ptr<Model> makeModel(std::string path, float p, float temp) {

	return std::make_shared<Model>(path, p, temp);
}

LoadingScene::LoadingScene(sf::Vector2f screenSize, std::shared_ptr<Game> game) : Scene(screenSize, game)
{
	/* *******************************************************************************
	*   Loading the llm model
	* ****************************************************************************/
	std::string model_path = "../PetSim/ressources/models/SmolLM2-1.7B-Instruct-IQ4_XS.gguf";
	mModelFuture = std::async(std::launch::async, makeModel, model_path, 0.2, 1.5);

	auto tm = TextureManager::getInstance();

	auto& background = mSceneObjects.at(SceneObject::BACKGROUND);
	background->setTexture(&tm->getTexture(Texture::LOADING_SCREEN));

	mSpinner = sf::RectangleShape({32,32});
	mSpinner.setPosition({ screenSize.x / 2, screenSize.y / 1.5f });
	mSpinner.setRotation(sf::degrees(10));
	mSpinner.setTexture(&tm->getTexture(Texture::SPRITESHEET));
	mSpinner.setTextureRect(sf::IntRect({ 126,0 }, { 32,32 }));
	mSpinner.setOrigin((sf::Vector2f)mSpinner.getLocalBounds().size / 2.f);
}

void LoadingScene::update(float dt)
{
	mElapsedTime += dt;

	if (mElapsedTime > 5) 
	{
		// Initialize the model
		mModel = mModelFuture.get();

		if (mModel)
		{
			SceneManager::getInstance()->replaceScene(std::make_shared<GameScene>(mScreenSize, mGame, mModel));
		}
	}

	mSpinner.rotate(sf::degrees(180*dt));
}


void LoadingScene::render(sf::RenderWindow& window)
{
	for (auto& obj : mSceneObjects)
	{
		window.draw(*obj.second);
	}

	window.draw(mSpinner);

	for (auto& txt : mSceneText)
	{
		window.draw(txt.second);
	}

}
