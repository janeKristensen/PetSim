#pragma once
#include "SFML/Graphics.hpp"


class IAnimated 
{
public:
	virtual ~IAnimated() {};
	virtual void setTexture(const sf::Texture& texture) = 0;
	virtual void setTexRect(sf::IntRect rect) = 0;
};

class Animation
{
public:
	Animation(const sf::Texture& texture, size_t frames, sf::Vector2i frameSize);
	Animation(const Animation&) = default;
	void update(float dt);
	const sf::Texture& getTexture() const { return mTexture; }
	sf::IntRect getFrame();

private:
	const sf::Texture& mTexture;
	sf::Vector2i mFrameSize;
	size_t mFrames;
	size_t mCurrentFrame = 0;
	float mFrameRate = 2.f;
	float mElapsedTime = 0.f;
	std::vector<sf::IntRect> mFrameBuffer;
};
