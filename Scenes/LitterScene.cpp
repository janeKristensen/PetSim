#include "LitterScene.h"
#include "GameScene.h"


LitterScene::LitterScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, GameScene& scene) 
	: Scene(screenSize, game), mGameScene(scene), mLitterBox(sf::Sprite(*TextureManager::getInstance()->getTexture(Texture::LITTER_BOX)))
{

	auto tm = TextureManager::getInstance();
	
	auto box_position = sf::Vector2f{ screenSize.x / 5, screenSize.y / 10 };
	mLitterBox.setPosition(box_position);
	
	int rows = 4;
	auto tex_size = mLitterBox.getTexture().getSize();
	auto cell_size = sf::Vector2f{ (float)tex_size.x/rows, (float)tex_size.y /rows};

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			mGrid[i][j] = sf::RectangleShape();
			mGrid[i][j].setSize(cell_size);
			mGrid[i][j].setPosition({ box_position.x + (tex_size.x/ rows * i), box_position.y + (j * tex_size.y / rows)});
			mGrid[i][j].setFillColor(sf::Color::Transparent);
			mGrid[i][j].setOutlineColor(sf::Color::Black);
			mGrid[i][j].setOutlineThickness(2);
			
		}
	}
	
#ifdef shader
	shaders = std::vector<std::string>
	{
		"Shaders/Shader1.frag"
	};

	if (!mShader.isAvailable())
	{
		std::cerr << "Shader not available";
		return;
	}

	if (!mShader.loadFromFile(shaders[0], sf::Shader::Type::Fragment))
	{
		std::cerr << "Shader not loaded!" << std::endl;
	}

	mRTA.setView(sf::View({ mScreenSize.x / 5, mScreenSize.y / 10 }, { 512.f,512.f }));
	mRTA.resize({ 512,512 });
	mRTA.clear(sf::Color::Transparent);
	mRTA.draw(mLitterBox);
	mRTA.display();

	mRTB.setView(sf::View({ mScreenSize.x / 5, mScreenSize.y / 10 }, { 512.f,512.f }));
	mRTB.resize({ 512,512 });
	mRTB.display();
#endif

	std::shared_ptr<sf::RectangleShape> menu = std::make_shared<sf::RectangleShape>(sf::Vector2f{
			screenSize.x - 2 * SCREEN_MARGIN,
			screenSize.y - 2 * SCREEN_MARGIN
		});
	menu->setTexture(tm->getTexture(Texture::TITLE_MENU).get());
	menu->setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });
	addSceneObject(SceneObject::START_MENU, menu);


	//Add particle manager to the litter box
	auto position = mLitterBox.getPosition();
	mBounds = {position , position + mBoxSize };
	mParticleManager = ParticleManager(mBounds);

	float rtn_btn_size = 32.f;
	std::shared_ptr<Command> return_cmd = std::make_shared<ContinueCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> return_btn = std::make_shared<Button>(sf::Vector2f{ rtn_btn_size, rtn_btn_size }, return_cmd);
	return_btn->setTexture(tm->getTexture(Texture::SPRITESHEET).get());
	return_btn->setTextureRect({ {64,0}, {(int)rtn_btn_size,(int)rtn_btn_size} });
	return_btn->setPosition({ screenSize.x - SCREEN_MARGIN - rtn_btn_size, SCREEN_MARGIN * 2 });
	addSceneObject(SceneObject::RETURN_BUTTON, return_btn);

	createParticles();
}

void LitterScene::createParticles()
{
	for (int i = 0; i < mBoxSize.x-1; i += mParticleSize)
	{
		for (int j = 0; j < mBoxSize.y-1; j += mParticleSize)
		{
			float x = mBounds.top_left.x + i;
			float y = mBounds.top_left.y + j;
			mParticleManager.addParticle(mParticleSize, 10, sf::Color::Yellow, { x,y }, mParticleSpeed);
		}
	}
	mVa.resize(mParticleManager.getParticles().size());
}

void LitterScene::update(float dt) 
{
#ifdef shader
	mShader.setUniform("currentTexture", mRTA.getTexture());
#endif
	
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

		auto btn = dynamic_pointer_cast<Button>(obj.second);
		if (btn)
		{
			mShader->setUniform("texture", sf::Shader::CurrentTexture);
			window.draw(*obj.second, btn->getShader().get());
		}
		else
		{
			window.draw(*obj.second);
		}
	}

	

#ifdef shader
	mRTB.clear(sf::Color::Red);
	sf::Sprite sprite(mRTA.getTexture());
	mRTB.draw(sprite, &mShader);
	mRTB.display();
	
	std::swap(mRTA, mRTB);

	mLitterBox.setTexture(mRTA.getTexture());
#endif

	window.draw(mLitterBox);
	
	auto& particles = mParticleManager.getParticles();
	
	for (size_t p = 0; p < particles.size(); ++p)
	{
		const Particle& particle = particles[p];

		// Position
		mVa[p].position = particle.getPosition();

		// Color (optional)
		mVa[p].color = sf::Color(210, 180, 73);
	}
	
	window.draw(mVa);

	for (auto& row : mGrid)
	{
		for (auto& cell : row)
		{
			window.draw(cell);
		}
	}
}

void LitterScene::handleClick(sf::Vector2f mouseposition)
{
	auto sm = SoundManager::getInstance();

	if (mSceneObjects.at(SceneObject::RETURN_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		sm->play(Sound::CLICK);
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::RETURN_BUTTON));
		btn->onClick();
	}
}

void LitterScene::handleKeyPress(sf::Keyboard::Key key) 
{

}

void LitterScene::handleDrag(std::shared_ptr<sf::RenderWindow> window)
{
	auto sm = SoundManager::getInstance();
	if (!sm->isPlaying())
	{
		sm->play(Sound::SAND);
	}

	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f position = window->mapPixelToCoords(pixelPos);
		
	mParticleManager.setDirection(position, mRadius);
	mParticleManager.setDirection(position + sf::Vector2f{ 0, 30 }, mRadius);
	mParticleManager.setDirection(position + sf::Vector2f{ 0, -30 }, mRadius);
		
	
#ifdef shader
	sf::Vector2f uv =
	{
		(position.x - mLitterBox.getPosition().x) / mLitterBox.getTexture().getSize().x,
		(position.y - mLitterBox.getPosition().y) / mLitterBox.getTexture().getSize().y
	};
	uv.y = 1 - uv.y;
	mShader.setUniform("u_mouse", uv);
#endif
		
}
