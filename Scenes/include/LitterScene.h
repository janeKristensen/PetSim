#pragma once
#include "scene.h"
#include "UserInterface.h"
#include "ParticleManager.h"


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
	void drag();

private:
	GameScene& mGameScene;
	Bounds mBounds;
	ParticleManager mParticleManager;
	sf::Vector2f mBoxSize{400,500};
	float mParticleSize = 5.f;
	size_t mParticlePoints = 4;
	float mParticleSpeed = 50.f;
	float mRadius = 7.5f;
	float mForce = 10.f;
};
