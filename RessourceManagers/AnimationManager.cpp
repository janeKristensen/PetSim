#include "AnimationManager.h"

AnimationManager* AnimationManager::mInstance = nullptr;


AnimationManager* AnimationManager::getInstance()
{
	if (mInstance == nullptr) mInstance = new AnimationManager();
	return mInstance;
}

void AnimationManager::update(float dt)
{
	for (auto& item : mAnimations)
	{
		item.second.update(dt);
		auto rect = item.second.getFrame();
		item.first->setTexRect(rect);
	}
}


void AnimationManager::addAnimation(AnimationName name, Animation animation)
{
	if (mAnimationMap.find(name) == mAnimationMap.end())
	{
		mAnimationMap.insert({ name, animation });
	}
}

void AnimationManager::attachAnimation(std::shared_ptr<IAnimated> object, AnimationName name)
{
	auto it = mAnimationMap.find(name);
	if (it != mAnimationMap.end())
	{
		const auto& animation = it->second;
		object->setTexture(animation.getTexture());
		mAnimations.insert({ object, animation });
	}
}

Animation& AnimationManager::getAnimation(AnimationName name)
{
	auto it = mAnimationMap.find(name);
	if (it != mAnimationMap.end())
	{
		return it->second;
	}
}