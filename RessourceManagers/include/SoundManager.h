#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

enum class Sound {
	CLICK,
	SAND
};

class SoundManager
{
public:
	SoundManager(SoundManager& other) = delete;
	void operator=(const SoundManager& other) = delete;

	static SoundManager* getInstance();
	void loadSound(Sound sound, const std::string& filename);
	const sf::SoundBuffer& getSoundBuffer(Sound sound);
	void play(Sound sound);
	bool isPlaying();
	void update();

protected:
	SoundManager() {};
	static SoundManager* mInstance;

private:
	
	std::unordered_map<Sound, sf::SoundBuffer> mSounds;
	std::vector<sf::Sound> mActiveSounds;
};