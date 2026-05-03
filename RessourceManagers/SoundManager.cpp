#include "SoundManager.h"


void SoundManager::loadSound(Sound sound, const std::string& filename)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(filename)) std::cout << "Error loading sound from file" << std::endl;;
	mSounds.insert({ sound, buffer });
}

const sf::SoundBuffer& SoundManager::getSoundBuffer(Sound sound)
{
	return mSounds[sound];
}

void SoundManager::play(Sound sound)
{
	mActiveSounds.push(sf::Sound(mSounds[sound]));
	mActiveSounds.back().play();
}

bool SoundManager::isPlaying()
{
	return mActiveSounds.size() > 0;
}

void SoundManager::update()
{
	while (!mActiveSounds.empty() && mActiveSounds.front().getStatus() == sf::SoundSource::Status::Stopped)
	{
		mActiveSounds.pop();
	}
}

