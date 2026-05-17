#include "ParticleManager.h"



ParticleManager::ParticleManager(Bounds bounds) : mBounds(bounds)
{
	mParticles.reserve(100000);
}

void ParticleManager::setBounds(Bounds bounds)
{
	mBounds = bounds;
}

void ParticleManager::addParticle(float size, size_t points, sf::Color color, sf::Vector2f position, float speed)
{
	auto particle = Particle(size, position);
	particle.setSpeed(speed);
	mParticles.push_back(particle);
}

void ParticleManager::setDirection(sf::Vector2f newPos, float radius)
{
	for (auto& particle : mParticles)
	{
		auto particle_position = particle.getPosition();
		auto direction = particle_position - newPos;

		// Compute length (magnitude)
		float length = direction.x * direction.x + direction.y * direction.y;

		if (length < radius * radius && length > 0)
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
	for (auto& particle : mParticles)
	{
		auto worldPos = particle.move(dt, force);
		sf::Vector2f newPos = particle.getPosition() + worldPos;
		if (newPos.x > mBounds.bottom_right.x) newPos.x = mBounds.bottom_right.x - particle.getRadius()*2;
		else if (newPos.x < mBounds.top_left.x) newPos.x = mBounds.top_left.x;
		if (newPos.y > mBounds.bottom_right.y) newPos.y = mBounds.bottom_right.y - particle.getRadius()*2;
		else if (newPos.y < mBounds.top_left.y) newPos.y = mBounds.top_left.y;
		particle.setPosition(newPos);
	}
}

void ParticleManager::resetDirection()
{
	for (auto& particle : mParticles)
	{
		particle.setDirection({ 0,0 });
	}
}


nlohmann::json ParticleManager::saveData()
{
	std::vector<nlohmann::json> particles;
	
	for (auto& particle : mParticles)
	{
		std::array<float, 3> position;
		position[0] = particle.getPosition().x;
		position[1] = particle.getPosition().y;
		position[2] = particle.getRadius();
		auto j = nlohmann::json{
		{"position", position}
		};
		particles.push_back(j);
	}	

	return particles;
}

void ParticleManager::loadData(nlohmann::json data)
{
	mParticles.clear();
	for (auto& element : data)
	{
		mParticles.push_back(Particle(element["position"][2], { element["position"][0], element["position"][1] }));
	}
}