#pragma once
#include <SFML/Audio.hpp>
#include <queue>
#include <iostream>

enum class Sound {
	CLICK,
	SAND,
	PICKUP,
	PLACE,
	LONG_PURR,
	SHORT_PURR
};

class SoundManager
{
public:
	SoundManager() {};
	void loadSound(Sound sound, const std::string& filename);
	const sf::SoundBuffer& getSoundBuffer(Sound sound);
	void play(Sound sound);
	bool isPlaying();
	void update();

private:
	std::unordered_map<Sound, sf::SoundBuffer> mSounds;
	std::queue<sf::Sound> mActiveSounds;
};