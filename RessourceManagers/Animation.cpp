#include "Animation.h"



Animation::Animation(std::shared_ptr<sf::Texture>  texture, size_t frames, sf::Vector2i startPosition, sf::Vector2i frameSize, float frameRate)
	: mTexture(texture), mFrames(frames), mFrameSize(frameSize), mFrameRate(frameRate)
{
	for (int i = 0; i < frames; i++)
	{
		auto texRect = sf::IntRect({ startPosition.x * i, startPosition.y }, { frameSize.x, frameSize.y });
		mFrameBuffer.push_back(texRect);
	}
	
}

void Animation::update(float dt)
{
	mElapsedTime += dt;
	if (mElapsedTime > mFrameRate)
	{
		mCurrentFrame++;
		if (mCurrentFrame == mFrames) mCurrentFrame = 0;
		mElapsedTime = 0;
	}
	
}

sf::IntRect Animation::getFrame()
{
	return mFrameBuffer[mCurrentFrame];
}

