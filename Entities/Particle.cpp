#include "Particle.h"

sf::Vector2f Particle::move(float dt, float force)
{
	m_velocity -= force * dt;
	if (m_velocity < 0) m_velocity = 0;
	return { m_direction.x * m_velocity * dt, m_direction.y * m_velocity * dt };
}
