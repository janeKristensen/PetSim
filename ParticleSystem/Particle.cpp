#include "Particle.h"

sf::Vector2f Particle::move(float dt, float force)
{
	sf::Vector2f vector{ m_direction.x * m_velocity * dt, m_direction.y * m_velocity * dt };
	m_velocity -= force;
	if (m_velocity < 0) m_velocity = 0;
	return vector;
}
