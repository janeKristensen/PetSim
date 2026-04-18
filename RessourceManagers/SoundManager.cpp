#include "SoundManager.h"

SoundManager* SoundManager::mInstance = nullptr;


SoundManager* SoundManager::getInstance()
{
	if (mInstance == nullptr) mInstance = new SoundManager();
	return mInstance;
}

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
	/*return std::find_if(mActiveSounds.begin(), mActiveSounds.end(),
		[](const sf::Sound& s)
		{
			return s.getStatus() == sf::Sound::Status::Playing;
		}) != mActiveSounds.end();*/
}

void SoundManager::update()
{
	if (mActiveSounds.size() > 1)
	{
		mActiveSounds.pop();
		/*mActiveSounds.erase(
			std::remove_if(mActiveSounds.begin(), mActiveSounds.end(),
				[](const sf::Sound& s)
				{
					return s.getStatus() == sf::Sound::Status::Stopped;
				}),
			mActiveSounds.end()
		);*/
	}
}

