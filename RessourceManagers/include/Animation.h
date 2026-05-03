#pragma once
#include "SFML/Graphics.hpp"


class IAnimated 
{
public:
	virtual ~IAnimated() {};
	virtual void setTexture(std::shared_ptr<sf::Texture> texture) = 0;
	virtual void setTexRect(sf::IntRect rect) = 0;
};

class Animation
{
public:
	Animation(std::shared_ptr<sf::Texture> texture, size_t frames, sf::Vector2i startPosition, sf::Vector2i frameSize, float frameRate);
	Animation(const Animation&) = default;
	
	void update(float dt);
	void resetFrame() { mCurrentFrame = 0; }
	std::shared_ptr<sf::Texture> getTexture() { return mTexture; }
	sf::IntRect getFrame();
	size_t getCurrentFrame() { return mCurrentFrame; }
	size_t getFrameNumber() { return mFrames; }

private:
	std::shared_ptr<sf::Texture> mTexture;
	sf::Vector2i mFrameSize;
	size_t mFrames;
	size_t mCurrentFrame = 0;
	float mFrameRate = 2.f;
	float mElapsedTime = 0.f;
	std::vector<sf::IntRect> mFrameBuffer;
};
