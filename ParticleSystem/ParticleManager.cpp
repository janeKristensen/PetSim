#include "ParticleManager.h"



ParticleManager::ParticleManager(Bounds bounds) : m_bounds(bounds)
{
	m_particles.reserve(100000);
}

void ParticleManager::setBounds(Bounds bounds)
{
	m_bounds = bounds;
}

void ParticleManager::addParticle(float size, size_t points, sf::Color color, sf::Vector2f position, float speed)
{
	auto particle = Particle(size, points);
	particle.setFillColor(color);
	particle.setPosition(position);
	particle.setSpeed(speed);
	m_particles.push_back(particle);
}

void ParticleManager::setDirection(sf::Vector2f position, float radius)
{
	for (auto& particle : m_particles)
	{
		auto direction = particle.getPosition() - position;
		
		// Compute length (magnitude)
		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length < radius)
		{
			direction.x /= length;
			direction.y /= length;

			particle.setVelocity(particle.getSpeed());
			particle.setDirection(direction);
		}
	}
}

void ParticleManager::moveParticles(float dt, float force)
{
	for (auto& particle : m_particles)
	{
		auto worldPos = particle.move(dt, force);
		sf::Vector2f newPos = particle.getPosition() + worldPos;
		if (newPos.x > m_bounds.bottom_right.x) newPos.x = m_bounds.bottom_right.x - particle.getRadius()*2;
		else if (newPos.x < m_bounds.top_left.x) newPos.x = m_bounds.top_left.x;
		if (newPos.y > m_bounds.bottom_right.y) newPos.y = m_bounds.bottom_right.y - particle.getRadius()*2;
		else if (newPos.y < m_bounds.top_left.y) newPos.y = m_bounds.top_left.y;
		particle.setPosition(newPos);
	}
}

void ParticleManager::resetDirection()
{
	for (auto& particle : m_particles)
	{
		particle.setDirection({ 0,0 });
	}
}
