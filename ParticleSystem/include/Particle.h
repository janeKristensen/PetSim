#pragma once
#include "SFML/Graphics.hpp"


class Particle 
{
public:
	Particle(float size, sf::Vector2f position) : m_radius(size), m_position(position){ }
	const sf::Vector2f getPosition() const { return m_position; }
	void setPosition(sf::Vector2f position) { m_position = position; }
	const float getRadius() const { return m_radius; }
	void setRadius(float size) { m_radius = size; }
	void setDirection(sf::Vector2f vector) { m_direction = vector; }
	sf::Vector2f getDirection() { return m_direction; }
	void setSpeed(float speed = 10) { m_speed = speed; }
	const float getSpeed() { return m_speed; }
	void setVelocity(float velocity) { m_velocity = velocity; }
	float getVelocity() { return m_velocity; }
	sf::Vector2f move(float dt, float force);
private:
	float m_speed;
	float m_velocity;
	float m_radius;
	sf::Vector2f m_direction;
	sf::Vector2f m_position;
};