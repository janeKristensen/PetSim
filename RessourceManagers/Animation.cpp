#include "Animation.h"



Animation::Animation(const sf::Texture& texture, size_t frames, sf::Vector2i frameSize) : mTexture(texture), mFrames(frames), mFrameSize(frameSize)
{
	for (int i = 0; i < frames; i++)
	{
		auto texRect = sf::IntRect({ frameSize.x * i, 0 }, { frameSize.x, frameSize.y });
		mFrameBuffer.push_back(texRect);
	}
	
}

void Animation::update(float dt)
{
	mElapsedTime += dt;
	if (mElapsedTime > mFrameRate)
	{
		mCurrentFrame++;
		if (mCurrentFrame == mFrames - 1) mCurrentFrame = 0;
		mElapsedTime = 0;
	}
	
}

sf::IntRect Animation::getFrame()
{
	return mFrameBuffer[mCurrentFrame];
}

