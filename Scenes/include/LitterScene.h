#pragma once
#include "scene.h"
#include "UserInterface.h"
#include "ParticleManager.h"
#include <cstdlib>

class GameScene;

class LitterScene : public Scene
{
public:
	LitterScene(sf::Vector2f screenSize, std::shared_ptr<Game> game, GameScene& scene);
	~LitterScene() {}
	void createParticles();
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;
	void handleClick(sf::Vector2f mouseposition) override;
	void handleKeyPress(sf::Keyboard::Key key) override;
	void handleDrag(std::shared_ptr<sf::RenderWindow> window) override;

private:
	void placeRandomPoops(size_t number);

	GameScene& mGameScene;
#ifdef  shader
	sf::RenderTexture mRTA = sf::RenderTexture();
	sf::RenderTexture mRTB = sf::RenderTexture();
	sf::Sprite mLitterBox = sf::Sprite(mRTA.getTexture());
	std::vector<std::string> shaders;
	sf::Shader mShader;
#endif 
	std::vector<std::shared_ptr<Item>> mItems;
	sf::Texture mParticleTexture;
	sf::Sprite mLitterBox;
	Bounds mBounds;
	ParticleManager mParticleManager;
	sf::VertexArray mVa = sf::VertexArray(sf::PrimitiveType::Points);
	std::array<std::array<sf::RectangleShape,4>, 4> mGrid;
	sf::Vector2f mBoxSize{512,512};
	sf::Vector2f mOldMousePos{0,0};
	float mParticleSize = 1.f;
	size_t mParticlePoints = 4;
	float mParticleSpeed = 50.f;
	float mRadius = 5.5f;
	float mForce = 50.f;
};
