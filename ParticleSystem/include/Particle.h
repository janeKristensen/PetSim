#pragma once
#include "SFML/Graphics.hpp"


class Particle : public sf::CircleShape
{
public:
	Particle(float size, size_t points) : CircleShape(size, points) { this->setOrigin({ size / 2, size / 2 }); }
	void setDirection(sf::Vector2f vector) { m_direction = vector; }
	sf::Vector2f getDirection() { return m_direction; }
	void setSpeed(float speed = 10) { m_speed = speed; }
	float getSpeed() { return m_speed; }
	void setVelocity(float velocity) { m_velocity = velocity; }
	float getVelocity() { return m_velocity; }
	sf::Vector2f move(float dt, float force);
private:
	float m_speed;
	float m_velocity;
	sf::Vector2f m_direction;
};