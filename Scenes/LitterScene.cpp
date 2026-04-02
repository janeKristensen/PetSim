#include "LitterScene.h"
#include "GameScene.h"


LitterScene::LitterScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, GameScene& scene) : Scene(screenSize, game), mGameScene(scene)
{
	auto tm = TextureManager::getInstance();

	std::shared_ptr<sf::RectangleShape> menu = std::make_shared<sf::RectangleShape>(sf::Vector2f{
			screenSize.x - 2 * SCREEN_MARGIN,
			screenSize.y - 2 * SCREEN_MARGIN
		});
	menu->setTexture(&tm->getTexture(Texture::TITLE_MENU));
	menu->setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });
	addSceneObject(SceneObject::START_MENU, menu);

	std::shared_ptr<sf::RectangleShape> litter_box = std::make_shared<sf::RectangleShape>(mBoxSize);
	litter_box->setFillColor(sf::Color::Black);
	litter_box->setPosition(
		{
			screenSize.x / 4,
			screenSize.y / 4
		}
	);
	addSceneObject(SceneObject::LITTER_BOX, litter_box);

	// Add particle manager to the litter box
	auto position = litter_box->getPosition();
	mBounds = {position , position + mBoxSize };
	mParticleManager = ParticleManager(mBounds);

	float rtn_btn_size = 32.f;
	std::shared_ptr<Command> return_cmd = std::make_shared<ContinueCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> return_btn = std::make_shared<Button>(sf::Vector2f{ rtn_btn_size, rtn_btn_size }, return_cmd);
	return_btn->setTexture(&tm->getTexture(Texture::SPRITESHEET));
	return_btn->setTextureRect({ {64,0}, {(int)rtn_btn_size,(int)rtn_btn_size} });
	return_btn->setPosition({ screenSize.x - SCREEN_MARGIN - rtn_btn_size, SCREEN_MARGIN * 2 });
	addSceneObject(SceneObject::RETURN_BUTTON, return_btn);

	createParticles();
}

void LitterScene::createParticles()
{
	for (int i = 0; i < mBoxSize.x-1; i += mParticleSize )
	{
		for (int j = 0; j < mBoxSize.y-1; j += mParticleSize )
		{
			float x = mBounds.top_left.x + i;
			float y = mBounds.top_left.y + j;
			mParticleManager.addParticle(mParticleSize, 10, sf::Color::Yellow, { x,y }, mParticleSpeed);
		}
	}
}

void LitterScene::update(float dt) 
{
	
	mParticleManager.moveParticles(dt, mForce);
	
}

void LitterScene::render(sf::RenderWindow& window) 
{
	window.draw(*mSceneObjects.at(SceneObject::BORDER));
	window.draw(*mSceneObjects.at(SceneObject::BACKGROUND));
	

	for (auto& obj : mSceneObjects)
	{
		if (obj.first == SceneObject::BACKGROUND ||
			obj.first == SceneObject::BORDER) continue;

		window.draw(*obj.second);
	}

	auto& particles = mParticleManager.getParticles();
	for (auto& particle : particles)
	{
		window.draw(particle);
	}
}

void LitterScene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::RETURN_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::RETURN_BUTTON));
		btn->onClick();
	}
}

void LitterScene::handleKeyPress(sf::Keyboard::Key key) 
{

}

void LitterScene::handleDrag(std::shared_ptr<sf::RenderWindow> window)
{
	sf::Clock timer = sf::Clock();
	timer.start();
	while (!mCurrentEvent.value().is<sf::Event::MouseButtonReleased>())
	{
		float elapsed = timer.getElapsedTime().asMilliseconds();
		if (elapsed > 30)
		{
			auto position = (sf::Vector2f)(sf::Mouse::getPosition(*window));

			mParticleManager.resetDirection();
			mParticleManager.setDirection(position, mRadius);
			mParticleManager.setDirection(position + sf::Vector2f{ 0, 30 }, mRadius);
			mParticleManager.setDirection(position + sf::Vector2f{ 0, -30 }, mRadius);

			timer.restart();
		}
	}
}
