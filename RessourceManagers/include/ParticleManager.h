#pragma once
#include "SFML/Graphics.hpp"
#include "Particle.h"
#include "nlohmann/json.hpp"


struct Bounds
{	
	Bounds(){}
	Bounds(sf::Vector2f top, sf::Vector2f bottom) : top_left(top), bottom_right(bottom){}
	sf::Vector2f top_left;
	sf::Vector2f bottom_right;
};


class ParticleManager
{
public:
	ParticleManager(){}
	ParticleManager(Bounds bounds);
	void setBounds(Bounds bounds);
	void addParticle(float size, size_t points, sf::Color color, sf::Vector2f position, float speed);
	void setDirection(sf::Vector2f newPos, float radius = 10);
	void moveParticles(float dt, float force);
	const std::vector<Particle>& getParticles() { return mParticles; }
	void resetDirection();
	nlohmann::json saveData();
	void loadData(nlohmann::json data);

private:
	Bounds mBounds;
	std::vector<Particle> mParticles;
};