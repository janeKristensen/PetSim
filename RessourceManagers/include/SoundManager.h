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

enum class Track {
	GAME_SCENE,
	SHOP_SCENE,
	LITTER_SCENE,
	WORK_SCENE,
	MENU,
	LOADING_SCREEN
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
	void newTrack(Track trackName, std::string filename);
	void playTrack(Track trackName, sf::Time offset);
	sf::Time saveTrackOffset();
	void pauseMusic();
	void stopMusic();
	void startMusic();

private:
	std::unordered_map<Sound, sf::SoundBuffer> mSounds;
	std::queue<sf::Sound> mActiveSounds;
	std::unordered_map<Track, std::string> mTracks;
	sf::Music mMusic;
};