#include "SoundManager.h"
#include <thread>

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

void SoundManager::newTrack(Track trackName, std::string filename)
{
	mTracks.insert({ trackName, filename });
}

void SoundManager::playTrack(Track trackName, sf::Time offset)
{
	mMusic.stop();
	auto track = mTracks.find(trackName);
	if (track != mTracks.end())
	{
		mMusic = sf::Music(track->second);
		mMusic.setVolume(70);
		mMusic.setLooping(true);
		mMusic.setPlayingOffset(offset);
		mMusic.play();	
	}
}

sf::Time SoundManager::saveTrackOffset()
{
	return mMusic.getPlayingOffset();
}

void SoundManager::pauseMusic()
{
	mMusic.pause();
}

void SoundManager::stopMusic()
{
	mMusic.stop();
}

void SoundManager::startMusic()
{
	mMusic.play();
}