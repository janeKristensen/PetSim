#include "LitterScene.h"
#include "GameScene.h"


LitterScene::LitterScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, Services& services, GameScene& scene)
	: Scene(screenSize, game, services), mGameScene(scene), mLitterBox(sf::Sprite(*services.textureManager->getTexture(Texture::LITTER_BOX)))
{
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
	menu->setTexture(mServices.textureManager->getTexture(Texture::TITLE_MENU).get());
	menu->setPosition({ SCREEN_MARGIN, SCREEN_MARGIN });
	addSceneObject(SceneObject::START_MENU, menu);


	//Add particle manager to the litter box
	auto position = mLitterBox.getPosition();
	mBounds = {position , position + mBoxSize };
	mParticleManager = ParticleManager(mBounds);

	float rtn_btn_size = 32.f;
	std::shared_ptr<Command> return_cmd = std::make_shared<ContinueCommand>(mGame);
	std::shared_ptr<sf::RectangleShape> return_btn = std::make_shared<Button>(sf::Vector2f{ rtn_btn_size, rtn_btn_size }, return_cmd);
	return_btn->setTexture(mServices.textureManager->getTexture(Texture::SPRITESHEET).get());
	return_btn->setTextureRect({ {64,0}, {(int)rtn_btn_size,(int)rtn_btn_size} });
	return_btn->setPosition({ screenSize.x - SCREEN_MARGIN - rtn_btn_size, SCREEN_MARGIN * 2 });
	addSceneObject(SceneObject::RETURN_BUTTON, return_btn);

	createParticles();

	// Add poop to the litterbox
	placeRandomPoops(3);

	// Create inventory of tools 
	mRake = std::make_unique<Rake>(ItemType::RAKE, Texture::SPRITESHEET, sf::IntRect({ 0,128 }, { 32,32 }), *mServices.textureManager->getTexture(Texture::SPRITESHEET), 0);
	mRake->setScale({2.0, 2.0});
	mRake->setPosition(mBounds.top_left - sf::Vector2f(42,0));

	mShovel = std::make_unique<Shovel>(ItemType::SHOVEL, Texture::SPRITESHEET, sf::IntRect({ 0,160 }, { 32,32 }), *mServices.textureManager->getTexture(Texture::SPRITESHEET), 0);
	mShovel->setScale({ 2.0, 2.0 });
	mShovel->setPosition(mBounds.top_left - sf::Vector2f(42,42));

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
	mItems.erase(std::remove(mItems.begin(), mItems.end(), nullptr), mItems.end());

	mTimeSinceLastPoop += dt;
	if (mTimeSinceLastPoop > 60) placeRandomPoops(1);
	mParticleManager.moveParticles(dt, mForce);
	scoopPoop();

	for (auto& item : mItems)
	{
		if (!item) continue;
		if (!item->isAlive()) item = nullptr;
	}
#ifdef shader
	mShader.setUniform("currentTexture", mRTA.getTexture());
#endif	
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
	for (auto& item : mItems)
	{
		if (!item) continue;
		window.draw(item->getSprite());
	}

	window.draw(mRake->getSprite());
	window.draw(mShovel->getSprite());

#ifndef NDEBUG
	for (auto& row : mGrid)
	{
		for (auto& cell : row)
		{
			window.draw(cell);
		}
	}
#endif // Debug

}

void LitterScene::handleClick(sf::Vector2f mouseposition)
{
	if (mSceneObjects.at(SceneObject::RETURN_BUTTON)->getGlobalBounds().contains(mouseposition))
	{
		mServices.soundManager->play(Sound::CLICK);
		auto btn = std::static_pointer_cast<Button>(mSceneObjects.at(SceneObject::RETURN_BUTTON));
		btn->onClick();
	}
}

void LitterScene::handleKeyPress(sf::Keyboard::Key key) 
{

}

void LitterScene::handleDrag(std::shared_ptr<sf::RenderWindow> window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f mouse_position = window->mapPixelToCoords(pixelPos);

	if (mRake->getSprite().getGlobalBounds().contains(mouse_position))
	{
		mServices.soundManager->play(Sound::PICKUP);
		
		while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) 
		{
			pixelPos = sf::Mouse::getPosition(*window);
			mouse_position = window->mapPixelToCoords(pixelPos);

			mRake->setPosition(mouse_position);
			if (!mServices.soundManager->isPlaying())
			{
				mServices.soundManager->play(Sound::SAND);
			}

			mParticleManager.setDirection(mouse_position, mRadius);
			mParticleManager.setDirection(mouse_position + sf::Vector2f{ 0, 30 }, mRadius);
			mParticleManager.setDirection(mouse_position + sf::Vector2f{ 0, -30 }, mRadius);
		}
	}
	else if (mShovel->getSprite().getGlobalBounds().contains(mouse_position))
	{
		mServices.soundManager->play(Sound::PICKUP);

		while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

			pixelPos = sf::Mouse::getPosition(*window);
			mouse_position = window->mapPixelToCoords(pixelPos);

			mShovel->setPosition(mouse_position);
		}

	}
	
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

void LitterScene::placeRandomPoops(size_t number)
{
	auto size = mLitterBox.getTexture().getSize();
	size_t randX;
	size_t randY;

	for (int i = 0; i < number; i++)
	{
		randX = std::rand() % (int)((mBounds.bottom_right.x-32) - mBounds.top_left.x) + mBounds.top_left.x;
		randY = std::rand() % (int)((mBounds.bottom_right.y-32) - mBounds.top_left.y) + mBounds.top_left.y;
		auto poop = std::make_shared<Poop>(ItemType::POOP, Texture::SPRITESHEET, sf::IntRect({ 128,160 }, { 32,32 }), *mServices.textureManager->getTexture(Texture::SPRITESHEET), 10);
		poop->setPosition({(float)randX, (float)randY});
		mItems.push_back(poop);
		mServices.needsSystem->processItem(*poop);
	}	
}

void LitterScene::scoopPoop()
{
	auto shovel_bounds = mShovel->getSprite().getGlobalBounds();
	for (auto& item : mItems)
	{
		if (item->getSprite().getGlobalBounds().findIntersection(shovel_bounds).has_value())
		{
			item->setAlive(false);
			mServices.soundManager->play(Sound::PICKUP);
		}
	}
}
