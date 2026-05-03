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
	tm->loadTexture(Texture::ANIMATION_SHEET, "ressources/assets/animationsheet.png");
	tm->loadTexture(Texture::GAME_BG, "ressources/assets/fixed_background_scaled.png");
	tm->loadTexture(Texture::INVENTORY, "ressources/assets/shelf.png");
	tm->loadTexture(Texture::LITTER_BOX, "ressources/assets/litter_box.png");

	FontManager::getInstance()->loadFont(FontName::TITLE, "ressources/fonts/Gabriola.ttf");

	auto sm = SoundManager::getInstance();
	sm->loadSound(Sound::PICKUP, "Sounds/pick_up.wav");
	sm->loadSound(Sound::PLACE, "Sounds/place.wav");
	sm->loadSound(Sound::SAND, "Sounds/sand_sound2.wav");
	sm->loadSound(Sound::LONG_PURR, "Sounds/long_low_purr.wav");
	sm->loadSound(Sound::SHORT_PURR, "Sounds/loud_purr.wav");

	auto& background = mSceneObjects.at(SceneObject::BACKGROUND);
	background->setTexture(tm->getTexture(Texture::LOADING_SCREEN).get());

	mSpinner = sf::RectangleShape({32,32});
	mSpinner.setPosition({ screenSize.x / 2, screenSize.y / 1.5f });
	mSpinner.setRotation(sf::degrees(10));
	mSpinner.setTexture(tm->getTexture(Texture::SPRITESHEET).get());
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
